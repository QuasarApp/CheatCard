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

namespace RC {

PermisionsModel::PermisionsModel(ImagesStorageModel * imageStorage):
    UsersListModel(imageStorage) {
    _waitModel = new WaitConfirmModel();
    QQmlEngine::setObjectOwnership(_waitModel, QQmlEngine::CppOwnership);
}

PermisionsModel::~PermisionsModel() {
    delete _waitModel;
}

void PermisionsModel::setPermissions(const QList<QSharedPointer<API::Contacts> > &newData,
                                     const QSharedPointer<API::User> &currentUser) {
    // clear old users list;
    _data.clear();
    setUsers({});


    for (const QSharedPointer<API::Contacts>& permision : newData) {
        _data.insert(permision->getGenesisKey(),
                     permision
                     );

        importUser(permision->toUser(currentUser));
    }
}

QObject *PermisionsModel::waitModel() const {
    return _waitModel;
}

void PermisionsModel::handleServerResult(const QSharedPointer<API::Contacts>& contact,
                                         const QSharedPointer<API::User> &currentUser,
                                         bool succesed, bool removed) {

    _waitModel->confirm(1, succesed);

    if (removed) {
        // to-do
    } else {
        _data.insert(contact->getGenesisKey(),
                     contact
                     );

        importUser(contact->toUser(currentUser));
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
        emit sigPermisionUpdated(_data[userModel->userId()]);
    }
}

void PermisionsModel::addNewPermision(const QString &description) {

    _waitModel->wait(1);

    emit sigPermisionAdded(description);
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
        emit sigPermisionRemoved(_data[userModel->userId()]);
    }

}
}
