//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cardmodel.h"
#include "aboutmodel.h"
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

#include <QDir>
#include <QGuiApplication>

#include <CheatCardGui/ibilling.h>

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

    qRegisterMetaType<RC::UsersCards>();
    qRegisterMetaType<RC::Card>();

    qRegisterMetaType<QSharedPointer<RC::UsersCards>>();
    qRegisterMetaType<QSharedPointer<RC::Card>>();

    auto app = dynamic_cast<QGuiApplication*>(QGuiApplication::instance());

    if (!app) {
        QuasarAppUtils::Params::log("The application required a QGuiApplication",
                                    QuasarAppUtils::Error);
    } else {
        // handle exit status on andorid
        QObject::connect(app, &QGuiApplication::applicationStateChanged,
                         this, &MainModel::handleAppStateChanged,
                         Qt::DirectConnection);

    }

    configureCardsList();
}

MainModel::~MainModel() {
    flush();

    delete _cardsListModel;
    delete _ownCardsListModel;

    if (_aboutModel) {
        delete _aboutModel;
    }

    delete _defaultLogosModel;
    delete _defaultBackgroundsModel;

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

QObject *MainModel::getAboutModel()
{
    if(!_aboutModel) {
        _aboutModel = new AboutModel();
    }
    return _aboutModel;
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
            _cardsListModel->setPurchasesNumbers(result->data());
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

#define r_prefix QString(":/images/private/resources/")

    QDir searcher(r_prefix + "/Icons");
    auto list = searcher.entryInfoList( QDir::Files, QDir::SortFlag::Name);

    QStringList tmpList;
    for (const auto &item : qAsConst(list)) {
        tmpList.push_back("qrc" + item.absoluteFilePath());
    }

    _defaultLogosModel->setStringList(tmpList);

    searcher.setPath(r_prefix + "/Backgrounds");
    list = searcher.entryInfoList(QDir::Files, QDir::SortFlag::Name);
    tmpList.clear();
    for (const auto &item : qAsConst(list)) {
        tmpList.push_back("qrc" + item.absoluteFilePath());
    }

    _defaultBackgroundsModel->setStringList(tmpList);


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
            this, &MainModel::handleListenStart, Qt::QueuedConnection);
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

void MainModel::initBilling(IBilling *billingObject) {

    if (!_currentUser) {
        QuasarAppUtils::Params::log("You try init billing before initialize user."
                                    " Please invoke the initBilling method after initialize user model.",
                                    QuasarAppUtils::Error);

        return;
    }

    if (_billing) {
        disconnect(_billing, &IBilling::sigPurchaseReceived,
                   this, &MainModel::handlePurchaseReceived);

        disconnect(_currentUser.data(), &UserModel::sigBecomeSeller,
                   _billing, &IBilling::becomeSeller);
    }

    _billing = billingObject;

    if (_billing) {
        connect(_billing, &IBilling::sigPurchaseReceived,
                this, &MainModel::handlePurchaseReceived);

        connect(_currentUser.data(), &UserModel::sigBecomeSeller,
                _billing, &IBilling::becomeSeller);

        _billing->init();
    }

}

void MainModel::flush() {
    saveConfig();
    saveUser();
}

int MainModel::getReceivedItemsCount(int cardId) const {
    if (_mode != Mode::Client) {
        return 0;
    }

    if (!_backEndModel)
        return 0;

    return _backEndModel->getCountOfReceivedItems(
                _currentUser->user()->userId(),
                cardId);
}

int MainModel::getMode() const {
    return static_cast<int>(_mode);
}

void RC::MainModel::configureCardsList() {
    if (_mode == Mode::Client) {
        setCardListModel(_cardsListModel);
        setBackEndModel(QSharedPointer<BaseNode>(new Visitor(_db), softRemove));
    } else {
        setCardListModel(_ownCardsListModel);
        setBackEndModel(QSharedPointer<BaseNode>(new Seller(_db), softRemove));
    }
}

void MainModel::setMode(int newMode) {
    if (static_cast<int>(_mode) == newMode)
        return;

    _mode = static_cast<Mode>(newMode);
    emit modeChanged();

    configureCardsList();

    _config->setFSellerEnabled(newMode);
    saveConfig();
}

QObject *MainModel::cardsList() const {
    return _currentCardsListModel;
}

void MainModel::handleCardReceived(QSharedPointer<RC::Card> card) {
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

void MainModel::handlePurchaseWasSuccessful(QSharedPointer<RC::UsersCards> card){

    int freeIndex;
    QSharedPointer<CardModel> cardModel;

    if (_mode == Mode::Client) {
        cardModel = _cardsListModel->cache().value(card->getCard()).model;
        _cardsListModel->setPurchasesNumbers({card});
        freeIndex = _backEndModel->getCardFreeIndex(card->getCard());

    } else {
        cardModel = _ownCardsListModel->cache().value(card->getCard()).model;

        freeIndex = _ownCardsListModel->cache().value(card->getCard()).source->getFreeIndex();
    }

    int freeItems = _backEndModel->getFreeItemsCount(card, freeIndex);
    _db->insertIfExistsUpdateObject(card);

    if (freeItems > 0) {
        emit freeItem(cardModel.data(), card->getUser(), freeItems);
    }
}

void MainModel::handleListenStart(int purchasesCount,
                                  QSharedPointer<CardModel> model,
                                  const QString& extraData) {

    auto header = QSharedPointer<UserHeader>::create();
    header->fromBytes(QByteArray::fromHex(extraData.toLatin1()));
    _lastUserHeader = header;

    sendSellerDataToServer(header, model->card()->cardId(), purchasesCount);
}

bool MainModel::sendSellerDataToServer(const QSharedPointer<UserHeader>& header,
                                       unsigned int cardId,
                                       int purchasesCount) {
    auto seller = _backEndModel.dynamicCast<Seller>();

    if (!seller)
        return false;

    if (!seller->incrementPurchase(header,
                                   cardId,
                                   purchasesCount)) {

        QuasarAppUtils::Params::log("Failed to increment user card data",
                                    QuasarAppUtils::Error);

        return false;
    }

    return true;
}

void MainModel::handleListenStop() {
}

void MainModel::handleAppStateChanged(Qt::ApplicationState state) {
    if (state == Qt::ApplicationState::ApplicationSuspended) {
        flush();
    }
}

void MainModel::handlePurchaseReceived(Purchase purchase) {
    if (purchase.token.isEmpty())
        return;

    if (!_currentUser) {
        return;
    }

    _currentUser->setSellerToken(QByteArray::fromBase64(purchase.token.toLatin1(),
                                          QByteArray::Base64UrlEncoding));

    initMode(_currentUser, _config);

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

void MainModel::handleBonusGivOut(int userId, int cardId, int count) {
    auto card = _backEndModel->getUserCardData(userId, cardId);

    if (card) {
        card->receive(count);

        _db->insertIfExistsUpdateObject(card);

        sendSellerDataToServer(_lastUserHeader, cardId, 0);
    }
}

QObject *MainModel::defaultLogosModel() const {
    return _defaultLogosModel;
}

QObject *MainModel::defaultBackgroundsModel() const {
    return _defaultBackgroundsModel;
}

}
