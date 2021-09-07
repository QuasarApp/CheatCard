//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "mainmodel.h"

#include "database.h"
#include "user.h"
#include "card.h"
#include "dbobjectsrequest.h"

#include "config.h"
#include "cardslistmodel.h"
#include "userscards.h"

#include <getsinglevalue.h>

namespace RC {

MainModel::MainModel(DB *db) {
    _db = db;
    _cardsListModel = new CardsListModel(_db);

    setCurrentUser(initUser());
    _config = initConfig(_currentUser->getId().toInt());

}

MainModel::~MainModel() {
    saveConfig();
    delete _cardsListModel;
}

bool MainModel::fFirst() const {
    if (!_config)
        return true;

    return _config->getFirstRun();
}

void MainModel::configureFinished() {
    // First run setiing id.

    auto actualUser = _db->getObject(*_currentUser.data());

    if (!actualUser)
        return;

    _config->setUserId(actualUser->getId().toInt());

    _config->setFirstRun(false);
}

QObject *MainModel::currentUser() const {
    return _currentUser.data();
}

void MainModel::setCurrentUser(User *newCurrentUser) {
    setCurrentUser(QSharedPointer<User>(newCurrentUser));
}

void MainModel::setCurrentUser(QSharedPointer<User> value) {

    if (_currentUser == value)
        return;

    if (_currentUser) {
        disconnect(_currentUser.data(), &User::nameChanged, this , &MainModel::handleUserChanged);
        disconnect(_currentUser.data(), &User::fSallerChanged, this , &MainModel::handleUserChanged);
    }

    _currentUser = value;

    if (_currentUser) {

        QString where = QString("Id IN (SELECT card FROM UsersCards WHERE user = %0 AND owner = %1)").
                arg(_currentUser->getId().toInt()).
                arg((_currentUser->fSaller())? "true": "false");

        QH::PKG::DBObjectsRequest<Card> request("Cards", where);
        if (auto result =_db->getObject(request)) {
            _cardsListModel->setCards(result->data());
        }

        connect(_currentUser.data(), &User::nameChanged, this , &MainModel::handleUserChanged);
        connect(_currentUser.data(), &User::fSallerChanged, this , &MainModel::handleUserChanged);
    }

    emit currentUserChanged();
}

void MainModel::handleUserChanged() {
    _db->insertIfExistsUpdateObject(_currentUser);
}

void MainModel::saveConfig() {
    _db->insertIfExistsUpdateObject(_config);
}

QSharedPointer<User> MainModel::initUser() {
    if (_currentUser) {
        return _currentUser;
    }

    QH::PKG::DBObjectsRequest<User> request("Users");

    auto result = _db->getObject(request);

    if (result && result->data().size()) {
        return QSharedPointer<User>(result->data().first());
    }    

    return QSharedPointer<User>::create();
}

QSharedPointer<Config> MainModel::initConfig(int userId) {
    if (_config) {
        return _config;
    }

    Config requestConfig;
    requestConfig.setUserId(userId);
    if (auto conf = _db->getObject(requestConfig)) {
        return conf;
    }

    return QSharedPointer<Config>::create(userId);
}

QObject *MainModel::cardsList() const {
    return _cardsListModel;
}

}
