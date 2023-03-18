//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "permisionsmodel.h"
#include "CheatCard/userheader.h"
#include "qrcodedecoder.h"
#include "rci/objects/icontacts.h"
#include "usermodel.h"
#include <QQmlEngine>
#include <qmlnotifyservice.h>
#include <rci/core/idb.h>
#include <rci/rcutils.h>

namespace RC {

PermisionsModel::PermisionsModel() {
    _codeProcessor = new QRCodeDecoder();

    QQmlEngine::setObjectOwnership(_codeProcessor, QQmlEngine::CppOwnership);

    connect(_codeProcessor, &QRCodeDecoder::decodeFinished,
            this, &PermisionsModel::handleImageDecodet);
}

PermisionsModel::~PermisionsModel() {
    delete _codeProcessor;
}

void PermisionsModel::setPermissions(const QList<QSharedPointer<Interfaces::iContacts> > &newData) {
    // clear old users list;
    _data.clear();
    setUsers({});


    for (const QSharedPointer<Interfaces::iContacts>& permision : newData) {
        _data.insert(permision->getChildUserKey(),
                     permision
                     );

        importUser(toUser(permision));
    }
}

QSharedPointer<Interfaces::iUser>
PermisionsModel::toUser(const QSharedPointer<Interfaces::iContacts>& contact) {
    if (auto _db = db()) {
        auto result = _db->makeEmptyUser();

        result->setKey(contact->getChildUserKey());
        result->setName(contact->getInfo());

        return result;
    }

    return nullptr;
}

void PermisionsModel::handleImageDecodet(const QString &data) {
    addNewPermision(data);
}

void PermisionsModel::setNewDescription(int row, const QString &description) {
    if (row >= rowCount()) {
        return;
    }

    // From QVariant to QObject *
    QObject* obj = qvariant_cast<QObject *>(data(index(row), UserObjectRole));
    // from QObject* to myClass*
    UserModel* userModel = qobject_cast<UserModel*>(obj);

    if (!userModel) {
        return;
    }

    userModel->setName(description);

    if (_data.contains(userModel->userKey())) {
        _data[userModel->userKey()]->setInfo(description);
        emit sigPermisionUpdated(_data[userModel->userKey()]);
    }
}

void PermisionsModel::addNewPermision(const QString& rawUserHeaderdata) {

    auto header = QSharedPointer<UserHeader>::create();
    header->fromBytes(QByteArray::fromHex(rawUserHeaderdata.toLatin1()));

    if (!header->isValid()) {
        auto service = QmlNotificationService::NotificationService::getService();
        service->setNotify(tr("Oops"),
                           tr("The Selected image does not contains a valid qr code. Try again."),
                           "", QmlNotificationService::NotificationData::Warning);

        return;
    }
    emit sigPermisionAdded(header);
}

void PermisionsModel::addNewPermisionFromImage(const QString &qrImage) {
    _codeProcessor->processQrCode(qrImage);
}

void PermisionsModel::removePermision(int row) {
    // From QVariant to QObject *
    QObject* obj = qvariant_cast<QObject *>(data(index(row), UserObjectRole));
    // from QObject* to myClass*
    UserModel* userModel = qobject_cast<UserModel*>(obj);
    if (!userModel) {
        return;
    }

    if (_data.contains(userModel->userKey())) {
        auto service = QmlNotificationService::NotificationService::getService();


        QmlNotificationService::Listner listner = [userModel, this] (bool accepted) {
            if (accepted) {
                emit sigPermisionRemoved(_data[userModel->userKey()]);
            }
        };

        service->setQuestion(listner, tr("Remove access for %0 ").arg(userModel->name()),
                           tr("If You remove access for the %0 user, he cannot be use your cards anymore.").
                             arg(userModel->name()) +
                           tr(" Do you want to continue?"),
                           "");

    }

}

}
