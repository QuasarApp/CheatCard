//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cardmodel.h"
#include "mainmodel.h"

#include "database.h"
#include "user.h"
#include "card.h"
#include "dbobjectsrequest.h"
#include "deleteobject.h"
#include "config.h"
#include "cardslistmodel.h"
#include "userscards.h"
#include "usermodel.h"
#include "userscards.h"
#include <getsinglevalue.h>

#include <QCoreApplication>

namespace RC {

MainModel::MainModel(DB *db) {
    _db = db;
    _cardsListModel = new CardsListModel();
    _ownCardsListModel = new CardsListModel();

    connect(_ownCardsListModel, &CardsListModel::sigCardAdded,
            this, &MainModel::handleCardCreated);

    connect(_ownCardsListModel, &CardsListModel::sigEditFinished,
            this, &MainModel::handleCardEditFinished);

    connect(_ownCardsListModel, &CardsListModel::sigCardRemoved,
            this, &MainModel::handleCardRemoved);

    setCurrentUser(initUser());
    _config = initConfig(_currentUser->user()->getId().toInt());

}

MainModel::~MainModel() {
    saveConfig();
    saveUser();
    delete _cardsListModel;
    delete _ownCardsListModel;

}

bool MainModel::fFirst() const {
    if (!_config)
        return true;

    return _config->getFirstRun();
}

void MainModel::configureFinished() {
    // First run setiing id.
    saveUser();

    auto actualUser = _db->getObject(*_currentUser->user().data());
    _currentUser->user()->setId(actualUser->getId());

    if (!actualUser)
        return;

    _config->setUserId(actualUser->getId().toInt());

    _config->setFirstRun(false);

    saveConfig();
}

QObject *MainModel::currentUser() const {
    return _currentUser.data();
}

void MainModel::setCurrentUser(UserModel *newCurrentUser) {
    setCurrentUser(QSharedPointer<UserModel>(newCurrentUser));
}

void MainModel::setCurrentUser(QSharedPointer<UserModel> value) {

    if (_currentUser == value)
        return;

    _currentUser = value;

    if (_currentUser) {

        QString where = QString("Id IN (SELECT card FROM UsersCards WHERE user = %0 AND owner = %1)").
                arg(_currentUser->user()->getId().toInt()).
                arg("true");

        QH::PKG::DBObjectsRequest<Card> request("Cards", where);
        if (auto result =_db->getObject(request)) {
            _ownCardsListModel->setCards(result->data());
        }


        where = QString("Id IN (SELECT card FROM UsersCards WHERE user = %0 AND owner = %1)").
                arg(_currentUser->user()->getId().toInt()).
                arg("false");

        request.setConditions(where);
        if (auto result =_db->getObject(request)) {
            _cardsListModel->setCards(result->data());
        }
    }

    emit currentUserChanged();
}

void MainModel::saveUser() {
    _db->insertIfExistsUpdateObject(_currentUser->user());
}

void MainModel::saveConfig() {
    _db->insertIfExistsUpdateObject(_config);
}

QSharedPointer<UserModel> MainModel::initUser() {
    if (_currentUser) {
        return _currentUser;
    }

    QH::PKG::DBObjectsRequest<User> request("Users");

    auto result = _db->getObject(request);

    if (result && result->data().size()) {
        return QSharedPointer<UserModel>::create(result->data().first());
    }    

    return QSharedPointer<UserModel>::create(QSharedPointer<User>::create());
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

QObject *MainModel::ownCardsList() const {
    return _ownCardsListModel;
}

void MainModel::handleCardCreated(QSharedPointer<CardModel> card) {
    _db->insertIfExistsUpdateObject(card->card());

    auto actualyCard = _db->getObject(*card->card().data());

    auto cards = QSharedPointer<UsersCards>::create(_currentUser->user()->getId().toInt(),
                                                    actualyCard->getId().toInt(), true);
    _db->insertIfExistsUpdateObject(cards);
}

void MainModel::handleCardEditFinished(const QSharedPointer<Card>& card) {
    _db->insertIfExistsUpdateObject(card);
}

void MainModel::handleCardRemoved(const QString &id) {

    QSharedPointer<Card> reqest;
    reqest->setName(id);

    _db->deleteObject(reqest);
}

}
