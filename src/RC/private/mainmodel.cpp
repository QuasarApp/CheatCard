//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cardmodel.h"
#include "itemsmodel.h"
#include "mainmodel.h"
#include "waitconnectionmodel.h"

#include <CheatCard/database.h>
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
#include "nfcbackend.h"
#include "cmath"

#define CURRENT_USER "CURRENT_USER"

namespace RC {

MainModel::MainModel(QH::ISqlDBCache *db) {
    _db = db;

    initCardsListModels();
    initImagesModels();
    initWaitConnectionModel();

    initBackEndModel();

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

    delete _backEndModel;

    delete _waitModel;
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

void MainModel::initCardsListModels() {
    _cardsListModel = new CardsListModel();
    _ownCardsListModel = new CardsListModel();

    connect(_ownCardsListModel, &CardsListModel::sigCardAdded,
            this, &MainModel::handleCardCreated);

    connect(_ownCardsListModel, &CardsListModel::sigEditFinished,
            this, &MainModel::handleCardEditFinished);

    connect(_ownCardsListModel, &CardsListModel::sigCardRemoved,
            this, &MainModel::handleCardRemoved);

    connect(_ownCardsListModel, &CardsListModel::sigCardSelectedForWork,
            this, &MainModel::handleCardSelectedForWork);
}

void MainModel::initImagesModels() {
    _defaultLogosModel = new ItemsModel();
    _defaultBackgroundsModel = new ItemsModel();

#define r_prefix QString("qrc:/images/private/resources/")
    _defaultLogosModel->setStringList({
                                          r_prefix + "/CoffeLogo.png",
                                          r_prefix + "/coffeSign.png",
                                          r_prefix + "/Icons/Cup_icon_1.png",
                                          r_prefix + "/Icons/Cup_icon_2.png",
                                          r_prefix + "/Icons/Cup_icon_3.png",
                                          r_prefix + "/Icons/Cup_icon_4.png",
                                          r_prefix + "/Icons/Cup_icon_5.png",
                                          r_prefix + "/Icons/Cup_icon_6.png",
                                          r_prefix + "/Icons/Cup_icon_7.png",
                                          r_prefix + "/Icons/Cup_icon_8.png",
                                          r_prefix + "/Icons/Cup_icon_9.png",
                                          r_prefix + "/Icons/Cup_icon_10.png",
                                          r_prefix + "/Icons/Cup_icon_11.png",
                                          r_prefix + "/Icons/Cup_icon_12.png",

                                      });

    _defaultBackgroundsModel->setStringList({
                                                r_prefix + "/Backgrounds/Layer_1.png",
                                                r_prefix + "/Backgrounds/Layer_2.png",
                                                r_prefix + "/Backgrounds/Layer_3.png",
                                                r_prefix + "/Backgrounds/Layer_4.png",
                                                r_prefix + "/Backgrounds/Layer_5.png",
                                                r_prefix + "/Backgrounds/Layer_6.png",
                                                r_prefix + "/Backgrounds/Layer_7.png",
                                                r_prefix + "/Backgrounds/Layer_8.png",
                                                r_prefix + "/Backgrounds/Layer_9.png",
                                                r_prefix + "/Backgrounds/Layer_10.png",
                                                r_prefix + "/Backgrounds/Layer_11.png",
                                                r_prefix + "/Backgrounds/Layer_12.png",
                                                r_prefix + "/Backgrounds/Layer_13.png",
                                                r_prefix + "/Backgrounds/Layer_14.png",
                                                r_prefix + "/Backgrounds/Layer_15.png",
                                                r_prefix + "/Backgrounds/Layer_16.png",
                                                r_prefix + "/Backgrounds/Layer_17.png",
                                                r_prefix + "/Backgrounds/Layer_18.png",
                                                r_prefix + "/Backgrounds/Layer_19.png",

                                            });


}

void MainModel::initBackEndModel() {
    _backEndModel = new NFCBackEnd(_db);
    connect(_backEndModel, &IConnectorBackEnd::sigSessionWasBegin,
            this, &MainModel::handleConnectWasBegin);

    connect(_backEndModel, &IConnectorBackEnd::sigSessionWasFinshed,
            this, &MainModel::handleConnectWasFinished);

    connect(_backEndModel, &IConnectorBackEnd::sigSessionWasFinshed,
            _waitModel, &WaitConnectionModel::handlePurchaseTaskFinished);

    connect(_backEndModel, &IConnectorBackEnd::sigPurchaseWasSuccessful,
            this, &MainModel::handlePurchaseWasSuccessful);

    connect(_backEndModel, &IConnectorBackEnd::sigCardReceived,
            this, &MainModel::handleCardReceived);
}

void MainModel::initWaitConnectionModel() {
    _waitModel = new WaitConnectionModel();

    connect(_waitModel, &WaitConnectionModel::purchaseTaskCompleted,
            this, &MainModel::handleListenStart);

    connect(_waitModel, &WaitConnectionModel::purchaseTaskCanceled,
            this, &MainModel::handleListenStop);
}

QObject *MainModel::waitModel() const {
    return _waitModel;
}

int MainModel::getMode() const {
    return _mode;
}

void MainModel::setMode(int newMode) {
    if (_mode == newMode)
        return;
    _mode = newMode;
    emit modeChanged();
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

void MainModel::handleCardReceived(QSharedPointer<Card> card) {
    _cardsListModel->importCard(card);
}

void MainModel::handleCardEditFinished(const QSharedPointer<Card>& card) {
    _db->insertIfExistsUpdateObject(card);
}

void MainModel::handleCardRemoved(int id) {

    QSharedPointer<Card> reqest;
    reqest->setId(id);

    _db->deleteObject(reqest);
}

void MainModel::handleCardSelectedForWork(const QSharedPointer<CardModel> &card) {
    _waitModel->setCard(card);
}

void MainModel::handleConnectWasBegin() {
    emit connectionWasBegin();
}

void MainModel::handleConnectWasFinished() {
    emit connectionWasEnd();
}

void MainModel::handlePurchaseWasSuccessful(QSharedPointer<UsersCards> card){

    int freeIndex;
    QSharedPointer<CardModel> cardModel;

    if (_backEndModel->mode() == IConnectorBackEnd::Mode::Client) {
        cardModel = _cardsListModel->cache().value(card->getCard()).model;
        _cardsListModel->setPurchasesNumbers({card});
        freeIndex = _cardsListModel->cache().value(card->getCard()).source->getFreeIndex();

    } else {
        cardModel = _ownCardsListModel->cache().value(card->getCard()).model;

        freeIndex = _ownCardsListModel->cache().value(card->getCard()).source->getFreeIndex();
    }

    int freeItems = card->getReceived() - std::ceil(card->getPurchasesNumber() / static_cast<double>(freeIndex));
    card->receive(freeItems);
    _db->insertIfExistsUpdateObject(card);

    emit freeItem(cardModel.data(), freeItems);
}

void MainModel::handleListenStart(int purchasesCount, QSharedPointer<CardModel> model) {
    _backEndModel->start(static_cast<IConnectorBackEnd::Mode>(_mode));
    _backEndModel->setActiveCard(model->card(), purchasesCount);
}

void MainModel::handleListenStop() {
    if (_backEndModel->mode() == IConnectorBackEnd::Saller) {
        _backEndModel->stop();
    }
}

QObject *MainModel::defaultLogosModel() const {
    return _defaultLogosModel;
}

QObject *MainModel::defaultBackgroundsModel() const {
    return _defaultBackgroundsModel;
}

}
