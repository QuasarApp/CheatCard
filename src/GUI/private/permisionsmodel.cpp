//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "permisionsmodel.h"
#include "usermodel.h"
#include "waitconfirmmodel.h"
#include <QQmlEngine>
#include <qmlnotifyservice.h>

namespace RC {

PermisionsModel::PermisionsModel(ImagesStorageModel * imageStorage):
    UsersListModel(imageStorage) {
    _waitModel = new WaitConfirmModel();
    QQmlEngine::setObjectOwnership(_waitModel, QQmlEngine::CppOwnership);
}

PermisionsModel::~PermisionsModel() {
    delete _waitModel;
}

void PermisionsModel::setPermissions(const QList<QSharedPointer<API::Contacts> > &newData) {
    // clear old users list;
    _data.clear();
    setUsers({});


    for (const QSharedPointer<API::Contacts>& permision : newData) {
        _data.insert(permision->getChildUserId(),
                     permision
                     );

        importUser(permision->toUser());
    }
}

QObject *PermisionsModel::waitModel() const {
    return _waitModel;
}

void PermisionsModel::handleServerResult(const QSharedPointer<API::Contacts>& contact,
                                         bool succesed, bool removed) {

    _waitModel->confirm(1, succesed);

    if (removed) {
        _data.remove(contact->getChildUserId());
        removeUser(contact->getChildUserId());
    } else {
        _data[contact->getChildUserId()] = contact;
        importUser(contact->toUser());
    }
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

    if (_data.contains(userModel->userId())) {
        _data[userModel->userId()]->setInfo(description);

        _waitModel->wait(1);

        emit sigPermisionUpdated(_data[userModel->userId()]);
    }
}

void PermisionsModel::addNewPermision(const QString& rawUserHeaderdata) {

    auto header = QSharedPointer<API::UserHeader>::create();
    header->fromBytes(QByteArray::fromHex(rawUserHeaderdata.toLatin1()));

    if (!header->isValid()) {
        return;
    }

    _waitModel->wait(1);

    emit sigPermisionAdded(header);
}

void PermisionsModel::removePermision(int row) {
    // From QVariant to QObject *
    QObject* obj = qvariant_cast<QObject *>(data(index(row), UserObjectRole));
    // from QObject* to myClass*
    UserModel* userModel = qobject_cast<UserModel*>(obj);
    if (!userModel) {
        return;
    }

    if (_data.contains(userModel->userId())) {
        auto service = QmlNotificationService::NotificationService::getService();


        QmlNotificationService::Listner listner = [userModel, this] (bool accepted) {
            if (accepted) {
                _waitModel->wait(1);
                emit sigPermisionRemoved(_data[userModel->userId()]);
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
