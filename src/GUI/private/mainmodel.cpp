//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cardmodel.h"
#include "aboutmodel.h"
#include "itemsmodel.h"
#include "mainmodel.h"
#include "qrcodereceiver.h"
#include "waitconnectionmodel.h"
#include "soundplayback.h"
#include "cardproxymodel.h"
#include "sellerstatisticmodel.h"

#include <CheatCard/database.h>

#include "CheatCard/api/api0/user.h"
#include "CheatCard/api/api0/card.h"
#include "CheatCard/api/api0/userscards.h"

#include "dbobjectsrequest.h"
#include "deleteobject.h"
#include "config.h"
#include "cardslistmodel.h"
#include "usermodel.h"
#include <getsinglevalue.h>

#include <QCoreApplication>
#include "cmath"
#include <qmlnotifyservice.h>

#include <CheatCard/sellerssl.h>
#include <CheatCard/visitorssl.h>

#include <QDir>
#include <QGuiApplication>

#include <CheatCardGui/ibilling.h>
#include "settingsmodel.h"
#include <CheatCard/api/apiv1.h>

#define CURRENT_USER "CURRENT_USER"

namespace RC {

void softRemove(BaseNode * obj) {
    obj->softDelete();
};

MainModel::MainModel(QH::ISqlDBCache *db) {
    _db = db;
    _soundEffect = new SoundPlayback;

    _config = dynamic_cast<SettingsModel*>(
                QuasarAppUtils::Settings::ISettings::instance());

    initCardsListModels();
    initImagesModels();
    initWaitConnectionModel();
    initSellerStatisticModel();

    configureCardsList();

    setCurrentUser(initUser());

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
    delete _soundEffect;
}

bool MainModel::fFirst() const {
    if (!_config)
        return true;

    return _config->getValue("fFirst", true).toBool();
}

void MainModel::configureFinished() {
    // First run setiing id.
    saveUser();
    _config->setCurrUser(getCurrentUser()->user()->userId());
    _config->setValue("fFirst", false);

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

const QSharedPointer<UserModel>& MainModel::getCurrentUser() const {
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

         if (_backEndModel) {
             _backEndModel->setCurrentUser(_currentUser->user());
         }

        _config->setCurrUser(_currentUser->user()->userId());

        _currentUser->regenerateSessionKey();

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
            _cardsListModel->updateMetaData(result->data());
        }

        _settings.setValue(CURRENT_USER, _currentUser->user()->userId());
    }

    emit currentUserChanged();
}

void MainModel::saveUser() {
    _db->insertIfExistsUpdateObject(_currentUser->user());
}

void MainModel::saveConfig() {
    _config->sync();
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

void MainModel::initCardsListModels() {
    _cardsListModel = new CardsListModel();
    _ownCardsListModel = new CardsListModel();

    _currentCardsListModel = new CardProxyModel();

    _currentCardsListModel->sort(0);
    _currentCardsListModel->setDynamicSortFilter(true);

    connect(_ownCardsListModel, &CardsListModel::sigEditFinished,
            this, &MainModel::handleCardEditFinished);

    connect(_ownCardsListModel, &CardsListModel::sigCardRemoved,
            this, &MainModel::handleCardRemoved);

    connect(_ownCardsListModel, &CardsListModel::sigCardSelectedForWork,
            this, &MainModel::handleCardSelectedForWork);

    connect(_ownCardsListModel, &CardsListModel::sigCardSelectedForStatistic,
            this, &MainModel::handleCardSelectedForStatistic);
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

void MainModel::initSellerStatisticModel() {
    _statisticModel = new SellerStatisticModel;
}

void MainModel::setCardListModel(CardsListModel *model) {
    if (_currentCardsListModel->sourceModel() == model)
        return;

    _currentCardsListModel->setSourceModel(model);
}

void MainModel::initMode(const QSharedPointer<UserModel> &user) {
    bool fSallerEnabled = _config && _config->getValue("fSellerMode", false).toBool();
    setMode(user && user->fSaller() && fSallerEnabled);
}

void MainModel::soundEffectPlayback(const QString &soundName) {
    _soundEffect->playSound(soundName);
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

QObject *MainModel::statisticModel() const {
    return _statisticModel;
}

int MainModel::getMode() const {
    return static_cast<int>(_mode);
}

void RC::MainModel::configureCardsList() {
    BaseNode* client = nullptr;

    if (_mode == Mode::Client) {
        setCardListModel(_cardsListModel);
        if (!_visitorbackEndModel) {
            _visitorbackEndModel = QSharedPointer<BaseNode>(new VisitorSSL(_db), softRemove);
            _visitorbackEndModel->addApiParser<ApiV1>();

            connect(_visitorbackEndModel.data(),
                    &BaseNode::sigNetworkError,
                    this,
                    &MainModel::handleNetworkError);
        }

        setBackEndModel(_visitorbackEndModel);
    } else {
        setCardListModel(_ownCardsListModel);
        if (!_sellerbackEndModel) {
            _sellerbackEndModel = QSharedPointer<BaseNode>(new SellerSSL(_db), softRemove);
            _sellerbackEndModel->addApiParser<ApiV1>();

            connect(_sellerbackEndModel.data(),
                    &BaseNode::sigNetworkError,
                    this,
                    &MainModel::handleNetworkError);
        }

        setBackEndModel(_sellerbackEndModel);
    }
}

void MainModel::setMode(int newMode) {
    if (static_cast<int>(_mode) == newMode)
        return;

    _mode = static_cast<Mode>(newMode);
    emit modeChanged();

    configureCardsList();

    _config->setValue("fSellerMode", static_cast<bool>(newMode));
    saveConfig();
}

QObject *MainModel::cardsList() const {
    return _currentCardsListModel;
}

void MainModel::handleCardReceived(QSharedPointer<RC::Card> card) {
    _cardsListModel->importCard(card);

    if (_backEndModel) {
        auto metaData = _backEndModel->getUserCardData(_currentUser->user()->userId(), card->cardId());

        if (metaData) {
            getCurrentListModel()->updateMetaData({metaData});
        }
    }
}

void RC::MainModel::saveCard(const QSharedPointer<Card>& card) {
    card->setCardVersion(card->getCardVersion() + 1);

    _db->insertIfExistsUpdateObject(card);
    auto cards = QSharedPointer<UsersCards>::create(_currentUser->user()->userId(),
                                                    card->cardId(), true);
    _db->insertIfExistsUpdateObject(cards);
}

void MainModel::handleCardEditFinished(const QSharedPointer<Card>& card) {

    card->setOwnerSignature(getCurrentUser()->user()->getKey());

    auto localCard = _backEndModel->getCard(card->cardId());

    if (localCard && localCard->compare(*card.data())) {
        return;
    }

    auto listOfUsers = _backEndModel->getAllUserFromCard(card->cardId(), false);

    if (localCard && listOfUsers.size() && localCard->getFreeIndex() != card->getFreeIndex()) {

        auto service = QmlNotificationService::NotificationService::getService();

        if (service) {

            QmlNotificationService::Listner listner = [card, localCard, this] (bool accepted) {
                getCurrentListModel()->importCard(localCard);
                saveCard(localCard);

                if (accepted) {

                    card->idGen();
                    getCurrentListModel()->importCard(card);
                    saveCard(card);
                }
            };

            service->setQuestion(listner, tr("Your customers already using this card!"),
                                 tr(" You trying to change the bonus rules."
                                    " These changes will be saved as a new card."
                                    " The old card continue work correctly and all customers data will be saved."
                                    " Do you want to continue?"));


        }

        return;
    }

    saveCard(card);
}

void MainModel::handleCardRemoved(int id) {

    auto reqest = QSharedPointer<Card>::create();
    reqest->setId(id);

    _db->deleteObject(reqest);
}

void MainModel::handleCardSelectedForWork(const QSharedPointer<CardModel> &card) {
    _waitModel->setCard(card);
}

void MainModel::handleCardSelectedForStatistic(const QSharedPointer<CardModel> &card) {
    auto usersList = _backEndModel->getAllUserFromCard(card->card()->cardId(), false);
    auto usersDataList = _backEndModel->getAllUserDataFromCard(card->card()->cardId(), false);

    _statisticModel->setDataList(card, usersList, usersDataList);
}

void MainModel::handleConnectWasBegin() {
    emit connectionWasBegin();
}

void MainModel::handleConnectWasFinished() {
    emit connectionWasEnd();
}

void MainModel::handlePurchaseWasSuccessful(QSharedPointer<RC::UsersCards> card){

    soundEffectPlayback("Seal");
    auto cardModel = getCurrentListModel()->cache().value(card->getCard());
    int freeIndex = _backEndModel->getCardFreeIndex(card->getCard());

    if (_mode == Mode::Client) {
        getCurrentListModel()->updateMetaData({card});
    }

    int freeItems = _backEndModel->getFreeItemsCount(card, freeIndex);
    _db->insertIfExistsUpdateObject(card);

    if (freeItems > 0) {
        emit freeItem(cardModel.data(), card->getUser(), freeItems);
        soundEffectPlayback("Bonus");
    } else {
        if (_mode == Mode::Seller && _fShowEmptyBonuspackaMessage) {
            _fShowEmptyBonuspackaMessage = false;

            auto service = QmlNotificationService::NotificationService::getService();

            service->setNotify(tr("Sorry but not"),
                               tr("This client do not have any bonuses. Sorry... "),
                               "", QmlNotificationService::NotificationData::Normal);

        }
    }
}

void MainModel::handleListenStart(int purchasesCount,
                                  QSharedPointer<CardModel> model,
                                  const QString& extraData) {

    auto header = QSharedPointer<UserHeader>::create();
    header->fromBytes(QByteArray::fromHex(extraData.toLatin1()));
    _lastUserHeader = header;

    sendSellerDataToServer(header, model->card()->cardId(), purchasesCount, false);
}

bool MainModel::sendSellerDataToServer(const QSharedPointer<UserHeader>& header,
                                       unsigned int cardId,
                                       int purchasesCount,
                                       bool sendOnly) {

    auto seller = _backEndModel.dynamicCast<Seller>();

    if (!seller)
        return false;

    bool sendResult = false;
    if (sendOnly) {
        sendResult = seller->sentDataToServerPurchase(header, cardId);
    } else {

        // show message about users bonuses. see handlePurchaseWasSuccessful function.
        _fShowEmptyBonuspackaMessage = !purchasesCount;
        sendResult = seller->incrementPurchase(header,
                                               cardId,
                                               purchasesCount);
    }

    if (!sendResult) {

        QuasarAppUtils::Params::log("Failed to increment user card data",
                                    QuasarAppUtils::Error);

        auto service = QmlNotificationService::NotificationService::getService();

        service->setNotify(tr("Oops"),
                           tr("Some kind of garbage happened when reading the qr code. Try again"),
                           "", QmlNotificationService::NotificationData::Warning);

        return false;
    }

    return true;
}

CardsListModel *MainModel::getCurrentListModel() const {
    return static_cast<CardsListModel*>(_currentCardsListModel->sourceModel());
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

    initMode(_currentUser);

}

void MainModel::handleNetworkError(QAbstractSocket::SocketError errorCode) {

    auto service = QmlNotificationService::NotificationService::getService();

    service->setNotify(tr("Oops"),
                       tr("Error: Network error occured on the :0 node. Message: Node found."),
                       "", QmlNotificationService::NotificationData::Warning);
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

        sendSellerDataToServer(_lastUserHeader, cardId, 0, true);
    }
}

QObject *MainModel::defaultLogosModel() const {
    return _defaultLogosModel;
}

QObject *MainModel::defaultBackgroundsModel() const {
    return _defaultBackgroundsModel;
}

}
