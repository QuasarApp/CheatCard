//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "mainmodel.h"

#include "database.h"
#include "user.h"
#include "dbobjectsrequest.h"

#include "config.h"

namespace RC {

MainModel::MainModel(DB *db) {
    _db = db;

    QH::PKG::DBObjectsRequest<User> request("Users", "*");

    auto result = _db->getObject(request);

    if (result && result->data().size()) {
        setCurrentUser(result->data().first());
        Config requestConfig;
        requestConfig.setUserId(_currentUser->getId().toInt());
        _config = _db->getObject(requestConfig);
    }
}

bool MainModel::fFirst() const {
    return _config && _config->getFirstRun();
}

QObject *MainModel::currentUser() const {
    return _currentUser.data();
}

void MainModel::setCurrentUser(User *newCurrentUser) {
    auto value = QSharedPointer<User>(newCurrentUser);

    if (_currentUser == value)
        return;
    _currentUser = value;
    emit currentUserChanged();
}

}
