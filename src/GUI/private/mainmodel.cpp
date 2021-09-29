//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cardmodel.h"
#include "itemsmodel.h"
#include "mainmodel.h"
#include "qrcodereceiver.h"
#include "waitconnectionmodel.h"

#include <CheatCard/database.h>
#include "CheatCard/user.h"
#include "CheatCard/card.h"
#include "dbobjectsrequest.h"
#include "deleteobject.h"
#include "config.h"
#include "cardslistmodel.h"
#include "CheatCard/userscards.h"
#include "usermodel.h"
#include <getsinglevalue.h>

#include <QCoreApplication>
#include "cmath"
#include <qmlnotifyservice.h>

#include <CheatCard/seller.h>
#include <CheatCard/visitor.h>

#define CURRENT_USER "CURRENT_USER"

namespace RC {

void softRemove(BaseNode * obj) {
    obj->softDelete();
};

MainModel::MainModel(QH::ISqlDBCache *db) {
    _db = db;

    initCardsListModels();
    initImagesModels();
    initWaitConnectionModel();

    setCurrentUser(initUser());
    _config = initConfig(_currentUser->user()->userId());

    initMode(_currentUser, _config);
}

MainModel::~MainModel() {
    saveConfig();
    saveUser();

    delete _cardsListModel;
    delete _ownCardsListModel;

    delete _defaultLogosModel;
    delete _defaultBackgroundsModel;

    delete _waitModel;

    delete _receiver;
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

    initMode(_currentUser, _config);

    saveConfig();
}

QObject *MainModel::currentUser() const {
    return _currentUser.data();
}

QSharedPointer<UserModel> MainModel::getCurrentUser() const {
    return _currentUser;
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
        QString where = QString("id IN (SELECT card FROM UsersCards WHERE user = %0 AND owner = %1)").
                arg(_currentUser->user()->userId()).
                arg("true");

        QH::PKG::DBObjectsRequest<Card> request("Cards", where);
        if (auto result =_db->getObject(request)) {
            _ownCardsListModel->setCards(result->data());
        }

        // get list of included cards
        where = QString("id IN (SELECT card FROM UsersCards WHERE user = %0 AND owner = %1)").
                arg(_currentUser->user()->userId()).
                arg("false");

        request.setConditions(where);
        if (auto result =_db->getObject(request)) {
            _cardsListModel->setCards(result->data());
        }

        // get list of cards usings statuses
        where = QString("user = %0 AND owner = %1").
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

void MainModel::setBackEndModel(const QSharedPointer<BaseNode>& newModel) {
    _backEndModel = newModel;

    connect(_backEndModel.data(), &BaseNode::sigPurchaseWasSuccessful,
            this, &MainModel::handlePurchaseWasSuccessful);

    connect(_backEndModel.data(), &BaseNode::sigCardReceived,
            this, &MainModel::handleCardReceived);
}

void MainModel::initWaitConnectionModel() {
    _waitModel = new WaitConnectionModel();

    connect(_waitModel, &WaitConnectionModel::purchaseTaskCompleted,
            this, &MainModel::handleListenStart);

    connect(_waitModel, &WaitConnectionModel::purchaseTaskCanceled,
            this, &MainModel::handleListenStop);
}

void MainModel::initReceiverModel() {
    _receiver = new QrCodeReceiver();

    connect(_receiver, &QrCodeReceiver::sigDataReceived,
            this, &MainModel::handleFirstDataReceived);

    connect(_receiver, &QrCodeReceiver::sigDataSendet,
            this, &MainModel::handleFirstDataReceived);
}

void MainModel::setCardListModel(CardsListModel *model) {
    if (_currentCardsListModel == model)
        return;

    _currentCardsListModel = model;
    emit cardsListChanged();
}

void MainModel::initMode(const QSharedPointer<UserModel> &user,
                         const QSharedPointer<Config> &config) {
    setMode(user && user->fSaller() && config && config->getFSellerEnabled());
}

QH::ISqlDBCache *MainModel::db() const {
    return _db;
}

QObject *MainModel::waitModel() const {
    return _waitModel;
}

int MainModel::getMode() const {
    return static_cast<int>(_mode);
}

void MainModel::setMode(int newMode) {
    if (static_cast<int>(_mode) == newMode)
        return;

    _mode = static_cast<Mode>(newMode);
    emit modeChanged();

    if (_mode == Mode::Client) {
        setCardListModel(_cardsListModel);
        setBackEndModel(QSharedPointer<BaseNode>(new Visitor(_db), softRemove));
    } else {
        setCardListModel(_ownCardsListModel);
        setBackEndModel(QSharedPointer<BaseNode>(new Seller(_db), softRemove));
    }

    _config->setFSellerEnabled(newMode);
    saveConfig();
}

QObject *MainModel::cardsList() const {
    return _currentCardsListModel;
}

void MainModel::handleCardReceived(QSharedPointer<Card> card) {
    _cardsListModel->importCard(card);
}

void MainModel::handleCardEditFinished(const QSharedPointer<Card>& card) {
    _db->insertIfExistsUpdateObject(card);
    auto cards = QSharedPointer<UsersCards>::create(_currentUser->user()->userId(),
                                                    card->cardId(), true);
    _db->insertIfExistsUpdateObject(cards);
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

    if (_mode == Mode::Client) {
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
    _lastActivedCard.purchasesCount = purchasesCount;
    _lastActivedCard.lastCard = model;
}

void MainModel::handleListenStop() {
    _lastActivedCard = {};
}

void MainModel::handleFirstDataReceived(QByteArray data) {

    if (_mode != Mode::Seller) {
        return;
    }

    auto seller = _backEndModel.dynamicCast<Seller>();

    if (!seller) {
        return;
    }

    if (!_lastActivedCard.lastCard)
        return;

    auto header = QSharedPointer<UserHeader>::create();
    header->fromBytes(data);

    seller->incrementPurchase(header,
                              _lastActivedCard.lastCard->card()->cardId(),
                              _lastActivedCard.purchasesCount);

}

void MainModel::handleFirstDataSendet() {
    if (_mode != Mode::Client) {
        return;
    }

    auto visitor = _backEndModel.dynamicCast<Visitor>();

    if (!visitor) {
        return;
    }

    visitor->checkCardData(_currentUser->getSessinon());
}

QObject *MainModel::defaultLogosModel() const {
    return _defaultLogosModel;
}

QObject *MainModel::defaultBackgroundsModel() const {
    return _defaultBackgroundsModel;
}

}
