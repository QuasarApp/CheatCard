//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cardmodel.h"
#include "itemsmodel.h"
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

#define CURRENT_USER "CURRENT_USER"

namespace RC {

MainModel::MainModel(DB *db) {
    _db = db;
    _cardsListModel = new CardsListModel();
    _ownCardsListModel = new CardsListModel();

    _defaultLogosModel = new ItemsModel();
    _defaultBackgroundsModel = new ItemsModel();

    _defaultLogosModel->setStringList({
                                          "qrc:/images/private/resources/CoffeLogo.png",
                                          "qrc:/images/private/resources/coffeSign.png"
                                      });

    _defaultBackgroundsModel->setStringList({
                                                ""
                                            });

    connect(_ownCardsListModel, &CardsListModel::sigCardAdded,
            this, &MainModel::handleCardCreated);

    connect(_ownCardsListModel, &CardsListModel::sigEditFinished,
            this, &MainModel::handleCardEditFinished);

    connect(_ownCardsListModel, &CardsListModel::sigCardRemoved,
            this, &MainModel::handleCardRemoved);

    setCurrentUser(initUser());
    _config = initConfig(_currentUser->user()->userId());

}

MainModel::~MainModel() {
    saveConfig();
    saveUser();

    delete _cardsListModel;
    delete _ownCardsListModel;

    delete _defaultLogosModel;
    delete _defaultBackgroundsModel;
}

bool MainModel::fFirst() const {
    if (!_config)
        return true;

    return _config->getFirstRun();
}

void MainModel::configureFinished() {
    // First run setiing id.
    saveUser();

    _config->setUserId(_currentUser->user()->userId());
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

        // get list of owned cards
        QString where = QString("Id IN (SELECT card FROM UsersCards WHERE user = %0 AND owner = %1)").
                arg(_currentUser->user()->userId()).
                arg("true");

        QH::PKG::DBObjectsRequest<Card> request("Cards", where);
        if (auto result =_db->getObject(request)) {
            _ownCardsListModel->setCards(result->data());
        }

        // get list of included cards
        where = QString("Id IN (SELECT card FROM UsersCards WHERE user = %0 AND owner = %1)").
                arg(_currentUser->user()->userId()).
                arg("false");

        request.setConditions(where);
        if (auto result =_db->getObject(request)) {
            _cardsListModel->setCards(result->data());
        }

        // get list of cards usings statuses
        where = QString("user = %0 AND owner = %1)").
                arg(_currentUser->user()->userId()).
                arg("false");

        QH::PKG::DBObjectsRequest<UsersCards> requestPurchase("UsersCards", where);
        if (auto result =_db->getObject(requestPurchase)) {
            _cardsListModel->setPurchasesNumbers(requestPurchase.data());
        }

        _settings.setValue(CURRENT_USER, _currentUser->user()->userId());
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

    User requestLstUser;
    requestLstUser.setId(_settings.value(CURRENT_USER).toUInt());

    auto lastUser = _db->getObject(requestLstUser);

    if (lastUser && lastUser->isValid()) {
        return QSharedPointer<UserModel>::create(lastUser);
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

    auto cards = QSharedPointer<UsersCards>::create(_currentUser->user()->userId(),
                                                    card->card()->cardId(), true);
    _db->insertIfExistsUpdateObject(cards);
}

void MainModel::handleCardEditFinished(const QSharedPointer<Card>& card) {
    _db->insertIfExistsUpdateObject(card);
}

void MainModel::handleCardRemoved(int id) {

    QSharedPointer<Card> reqest;
    reqest->setId(id);

    _db->deleteObject(reqest);
}

QObject *MainModel::defaultLogosModel() const {
    return _defaultLogosModel;
}

QObject *MainModel::defaultBackgroundsModel() const {
    return _defaultBackgroundsModel;
}

}
