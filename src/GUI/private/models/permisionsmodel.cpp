//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "permisionsmodel.h"
#include "CheatCard/userheader.h"
#include "models/clientmodel.h"
#include "qrcodedecoder.h"
#include "rci/core/imodelsstorage.h"
#include "rci/objects/icontacts.h"
#include "imagelogomodel.h"
#include "usermodel.h"
#include "waitconfirmmodel.h"
#include <CheatCard/usersnames.h>
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

int PermisionsModel::rowCount(const QModelIndex &) const {
    return _data.size();
}

QVariant PermisionsModel::data(const QModelIndex &index, int role) const {
    if (index.row() >= rowCount()) {
        return {};
    }

    switch (role) {

    case DefaultAvatar: {
        if (!_defaultAvatars)
            return "";

        auto userKey = _permissions.value(index.row());
        return _defaultAvatars->getImageByHash(userKey);
    }

    case PermisionKey: {
        return _permissions.value(index.row());
    }

    case PermisionDescriptionRole: {
        auto userKey = _permissions.value(index.row());
        return _data.value(userKey)->getInfo();
    }

    default:
        return {};
    }
}

QHash<int, QByteArray> PermisionsModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[PermisionDescriptionRole] = "permisionDescription";
    roles[DefaultAvatar] = "defaultAvatar";
    roles[PermisionKey] = "permisionKey";

    return roles;
}

void PermisionsModel::setPermissions(const QList<QSharedPointer<Interfaces::iContacts> > &newData) {
    beginResetModel();

    _data.clear();

    for (const QSharedPointer<Interfaces::iContacts>& permision : newData) {

        _data.insert(permision->getChildUserKey(),
                     permision
                     );
        _permissions.push_back(permision->getChildUserKey());

    }

    endResetModel();
}

void PermisionsModel::handleCurrentUserChanged(const QSharedPointer<UserModel> &user) {
    _currentUserModel = user;
    setPermissions(db()->getSlaveKeys(user->userKey()));

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

void PermisionsModel::setNewDescription(QByteArray userKey, const QString &description) {
    auto permision = _data.value(userKey);
    if (!permision)
        return;

    auto backEndModel = storage()->get<ClientModel>();
    auto usersModel = storage()->get<UsersListModel>();
    auto waitModel = storage()->get<WaitConfirmModel>();

    if (backEndModel && usersModel && waitModel) {
        int watCode = rand();
        permision->setInfo(description);

        backEndModel->updateContactData(permision,
                                        usersModel->currentUserSecret(),
                                        false,
                                        [waitModel, watCode, usersModel, permision, description, userKey, this](auto err){
                                            waitModel->confirm(watCode, !err);
                                            if (!err) {
                                                int row = _permissions.indexOf(userKey);
                                                emit dataChanged(index(row, 0), index(row, 0), {PermisionDescriptionRole});
                                            }
                                        });
        waitModel->wait(watCode);

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

    auto backEndModel = storage()->get<ClientModel>();
    auto usersModel = storage()->get<UsersListModel>();
    auto waitModel = storage()->get<WaitConfirmModel>();
    if (backEndModel && usersModel && waitModel) {
        auto permision = db()->makeEmptyContact();
        auto info = header->userName();
        if (info.isEmpty()) {
            info = UsersNames::randomUserName();
        }

        if (!RCUtils::createContact(_currentUserModel->userKey(), header->userKey(), info , permision)) {
            return;
        }

        int watCode = rand();
        backEndModel->updateContactData(permision,
                                        _currentUserModel->user()->secret(),
                                        false,
                                        [waitModel, watCode, usersModel, permision, this](auto err){
                                            waitModel->confirm(watCode, !err);
                                            if (!err) {
                                                QMetaObject::invokeMethod(this,
                                                                          "addNewPermisionPrivate",
                                                                          Qt::QueuedConnection,
                                                                          Q_ARG(QSharedPointer<RC::Interfaces::iContacts>,
                                                                                permision));
                                            }

                                        });
        waitModel->wait(watCode);

    }
}

void PermisionsModel::addNewPermisionFromImage(const QString &qrImage) {
    _codeProcessor->processQrCode(qrImage);
}

void PermisionsModel::removePermision(QByteArray userKey) {
    auto permision = _data.value(userKey);
    if (!permision)
        return;

    auto backEndModel = storage()->get<ClientModel>();
    auto usersModel = storage()->get<UsersListModel>();
    auto waitModel = storage()->get<WaitConfirmModel>();

    if (backEndModel && usersModel && waitModel) {
        int watCode = rand();
        backEndModel->updateContactData(permision,
                                        usersModel->currentUserSecret(),
                                        true,
                                        [waitModel, watCode, usersModel, userKey, this](auto err){
                                            waitModel->confirm(watCode, !err);
                                            if (!err) {
                                                QMetaObject::invokeMethod(this,
                                                                          "removePermisionPrivate",
                                                                          Qt::QueuedConnection,
                                                                          Q_ARG(QByteArray,
                                                                                userKey));
                                            }
                                        });
        waitModel->wait(watCode);

    }
}

void PermisionsModel::init(const QSharedPointer<Interfaces::iDB> &db, const QSharedPointer<Interfaces::iModelsStorage> &global) {
    BaseModel::init(db, global);
    _defaultAvatars = global->get(typeid(ImageLogoModel).hash_code()).staticCast<ImagesStorageModel>();
}

void PermisionsModel::addNewPermisionPrivate(QSharedPointer<Interfaces::iContacts> permision) {
    beginInsertRows({}, _permissions.size(), _permissions.size());

    _data.insert(permision->getChildUserKey(), permision);
    _permissions.push_back(permision->getChildUserKey());

    endInsertRows();
}

void PermisionsModel::removePermisionPrivate(QByteArray permision) {
    int index = _permissions.indexOf(permision);

    if (index < 0)
        return;

    beginRemoveRows({}, index, index);

    _data.remove(permision);
    _permissions.removeAt(index);

    endRemoveRows();
}

}
