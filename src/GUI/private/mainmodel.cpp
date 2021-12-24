//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cardmodel.h"
#include "netindicatormodel.h"
#include "aboutmodel.h"
#include "itemsmodel.h"
#include "mainmodel.h"
#include "qrcodereceiver.h"
#include "waitconnectionmodel.h"
#include "soundplayback.h"
#include "cardproxymodel.h"
#include "sellerstatisticmodel.h"
#include "importexportusermodel.h"

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


namespace RC {

void softRemove(BaseNode * obj) {
    obj->softDelete();
};

MainModel::MainModel(QH::ISqlDBCache *db) {
    _db = db;
    _soundEffect = new SoundPlayback;
    _config = dynamic_cast<SettingsModel*>(
                QuasarAppUtils::Settings::ISettings::instance());

    _config->setCurrUser(_settings.value(CURRENT_USER).toUInt());

    initCardsListModels();
    initImagesModels();
    initWaitConnectionModel();
    initSellerStatisticModel();
    initImportExportModel();
    initNetIndicateModels();

    configureCardsList();

    setCurrentUser(initUser());

    qRegisterMetaType<RC::API::UsersCards>();
    qRegisterMetaType<RC::API::Card>();

    qRegisterMetaType<QSharedPointer<RC::API::UsersCards>>();
    qRegisterMetaType<QSharedPointer<RC::API::Card>>();

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

    delete  _statisticModel;
    delete _netIdicatorModel;

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
    _currentUser->regenerateSessionKey();
}

QObject *MainModel::getAboutModel()
{
    if(!_aboutModel) {
        _aboutModel = new AboutModel();
    }
    return _aboutModel;
}

QObject *MainModel::getNetIndicatorModel() const {
    return _netIdicatorModel;
}

QObject *MainModel::currentUser() const {
    return _currentUser.data();
}

bool MainModel::handleImportUser(const QString &base64UserData) {

    auto userData = QSharedPointer<API::User>::create();
    userData->fromBytes(QByteArray::fromBase64(base64UserData.toLatin1(),
                                               QByteArray::Base64UrlEncoding));


    auto service = QmlNotificationService::NotificationService::getService();

    auto userKey = userData->getKey();
    auto secret = userData->secret();
    if (userKey != API::User::makeKey(secret)) {

        service->setNotify(tr("We Has a troubles"),
                           tr("Yor secret key and public key is not pair"
                              " May be you scan not valid qr code ..."),
                           "", QmlNotificationService::NotificationData::Error);
        return false;
    }

    if (!userData->isValid()) {

        if (service) {

            service->setNotify(tr("Wow shit"),
                               tr("This qr code is invalid. Sorry... "),
                               "", QmlNotificationService::NotificationData::Error);
        }


        return false;
    }

    auto newUser = QSharedPointer<UserModel>::create(userData);

    setCurrentUser(newUser);
    saveUser();
    _config->setValue("fFirst", false);

    if (!_backEndModel->restoreOldData(userData->getKey())) {
        service->setNotify(tr("We Has a troubles"),
                           tr("Yor secret key are imported successful but donwload backup data from server is failed."
                              " Please check your internet connection and try restore your data again"),
                           "", QmlNotificationService::NotificationData::Warning);

        return false;
    }

    service->setNotify(tr("I managed to do it !"),
                       tr("Yor secret key are imported"),
                       "", QmlNotificationService::NotificationData::Normal);


    return true;
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

        if (_visitorbackEndModel) {
            _visitorbackEndModel->setCurrentUser(_currentUser->user());
        }

        if (_sellerbackEndModel) {
            _sellerbackEndModel->setCurrentUser(_currentUser->user());
        }

        unsigned int userId = _currentUser->user()->userId();
        QByteArray userKey = _currentUser->user()->getKey();

        _config->setCurrUser(userId);

        _currentUser->regenerateSessionKey();

        // get list of owned cards
        _ownCardsListModel->setCards(_backEndModel->getAllUserCards(userKey,
                                                                    false));

        // get list of included cards
        _cardsListModel->setCards(_backEndModel->getAllUserCards(userKey,
                                                                 true));

        // get list of cards usings statuses
        QString where = QString("user = %0").
                arg(userId);

        QH::PKG::DBObjectsRequest<API::UsersCards> requestPurchase("UsersCards", where);
        if (auto result =_db->getObject(requestPurchase)) {
            _cardsListModel->updateMetaData(result->data());
        }

        _settings.setValue(CURRENT_USER, userId);

        if (_billing) {
            connect(_currentUser.data(), &UserModel::sigBecomeSeller,
                    _billing, &IBilling::becomeSeller);
        }
    }

    emit currentUserChanged();
}

void MainModel::saveUser() {
    _db->insertIfExistsUpdateObject(_currentUser->user());
    _settings.setValue(CURRENT_USER, _currentUser->user()->userId());
}

void MainModel::saveConfig() {
    _config->sync();
}

QSharedPointer<UserModel> MainModel::initUser() {
    if (_currentUser) {
        return _currentUser;
    }

    API::User requestLstUser;
    requestLstUser.setId(_settings.value(CURRENT_USER).toUInt());

    auto lastUser = _db->getObject(requestLstUser);

    if (lastUser && lastUser->isValid()) {
        return QSharedPointer<UserModel>::create(lastUser);
    }

    QH::PKG::DBObjectsRequest<API::User> request("Users");

    auto result = _db->getObject(request);

    if (result && result->data().size()) {
        return QSharedPointer<UserModel>::create(result->data().first());
    }

    return QSharedPointer<UserModel>::create(QSharedPointer<API::User>::create());
}

void MainModel::initCardsListModels() {
    _cardsListModel = new CardsListModel();
    _ownCardsListModel = new CardsListModel();

    _currentCardsListModel = new CardProxyModel();

    _currentCardsListModel->sort(0);
    _currentCardsListModel->setDynamicSortFilter(true);

    connect(_cardsListModel, &CardsListModel::sigRemoveRequest,
            this, &MainModel::handleRemoveRequest);

    connect(_ownCardsListModel, &CardsListModel::sigEditFinished,
            this, &MainModel::handleCardEditFinished);

    connect(_cardsListModel, &CardsListModel::sigEditFinished,
            this, &MainModel::saveCard);

    connect(_ownCardsListModel, &CardsListModel::sigRemoveRequest,
            this, &MainModel::handleRemoveRequest);

    connect(_ownCardsListModel, &CardsListModel::sigCardSelectedForWork,
            this, &MainModel::handleCardSelectedForWork);

    connect(_ownCardsListModel, &CardsListModel::sigCardSelectedForStatistic,
            this, &MainModel::handleCardSelectedForStatistic);

    connect(_cardsListModel, &CardsListModel::sigResetCardModel,
            this, &MainModel::handleResetCardModel);

    connect(_ownCardsListModel, &CardsListModel::sigResetCardModel,
            this, &MainModel::handleResetCardModel);
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

void MainModel::initNetIndicateModels() {
    _netIdicatorModel = new NetIndicatorModel();
}

void MainModel::setBackEndModel(const QSharedPointer<BaseNode>& newModel) {

    if (_backEndModel) {
        disconnect(_backEndModel.data(),
                   &BaseNode::sigAvailableNetworkChanged,
                   static_cast<NetIndicatorModel*>(getNetIndicatorModel()),
                   &NetIndicatorModel::handleEndaleNetworkChanged);

        disconnect(_backEndModel.data(), &BaseNode::sigPurchaseWasSuccessful,
                   this, &MainModel::handlePurchaseWasSuccessful);

        disconnect(_backEndModel.data(), &BaseNode::sigCardReceived,
                   this, &MainModel::handleCardReceived);
    }

    _backEndModel = newModel;

    if (_backEndModel) {

        connect(_backEndModel.data(),
                &BaseNode::sigAvailableNetworkChanged,
                static_cast<NetIndicatorModel*>(getNetIndicatorModel()),
                &NetIndicatorModel::handleEndaleNetworkChanged);

        connect(_backEndModel.data(), &BaseNode::sigPurchaseWasSuccessful,
                this, &MainModel::handlePurchaseWasSuccessful);

        connect(_backEndModel.data(), &BaseNode::sigCardReceived,
                this, &MainModel::handleCardReceived);

        _backEndModel->checkNetworkConnection();

    }
}

void MainModel::initWaitConnectionModel() {
    _waitModel = new WaitConnectionModel();

    connect(_waitModel, &WaitConnectionModel::purchaseTaskCompleted,
            this, &MainModel::handleListenStart, Qt::QueuedConnection);
}

void MainModel::initSellerStatisticModel() {
    _statisticModel = new SellerStatisticModel;
}

void MainModel::initImportExportModel() {
    if (!_importExportModel) {
        _importExportModel = new ImportExportUserModel();
        emit exportImportModelChanged();

        connect(_importExportModel, &ImportExportUserModel::decodeFinished,
                this, &MainModel::handleImportUser);
    }
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

// template method for initialize back end model. using in configureCardsList method
template <class BackEndType>
QSharedPointer<BaseNode> initBackEndModel(const QSharedPointer<UserModel>& user,
                                          QH::ISqlDBCache *db) {
    QSharedPointer<BaseNode> result;
    result = QSharedPointer<BaseNode>(new BackEndType(db), softRemove);
    result->addApiParser<ApiV1>();

    if (user) {
        result->setCurrentUser(user->user());
    }

    return result;
};

void RC::MainModel::configureCardsList() {
    if (_mode == Mode::Client) {
        setCardListModel(_cardsListModel);
        if (!_visitorbackEndModel) {
            _visitorbackEndModel = initBackEndModel<VisitorSSL>(_currentUser,
                                                                _db);
        }

        setBackEndModel(_visitorbackEndModel);
    } else {
        setCardListModel(_ownCardsListModel);
        if (!_sellerbackEndModel) {
            _sellerbackEndModel = initBackEndModel<SellerSSL>(_currentUser,
                                                              _db);
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

    if (_mode == Mode::Seller) {
        // test secret keys

        auto userKey = _currentUser->user()->getKey();
        auto secret = _currentUser->user()->secret();
        if (userKey != API::User::makeKey(secret)) {
            _currentUser->user()->regenerateKeys();
            saveUser();
            _settings.setValue(CURRENT_USER, _currentUser->user()->userId());
        }
    }

    saveConfig();
}

QObject *MainModel::cardsList() const {
    return _currentCardsListModel;
}

void MainModel::handleCardReceived(QSharedPointer<RC::API::Card> card) {

    if (card->isOvner(_currentUser->user()->userId())) {
        _ownCardsListModel->importCard(card);

    } else {
        if (!_cardsListModel)
            return;

        _cardsListModel->importCard(card);
        if (_backEndModel) {
            auto metaData = _backEndModel->getUserCardData(_currentUser->user()->userId(), card->cardId());

            if (metaData) {
                _cardsListModel->updateMetaData({metaData});
            }
        }
    }
}

void RC::MainModel::saveCard(const QSharedPointer<API::Card>& card) {
    if (getMode() == static_cast<int>(Mode::Client)) {
        card->setCardVersion(VERSION_CARD_USER);
    } else {
        card->setCardVersion(card->getCardVersion() + 1);
    }

    _db->insertIfExistsUpdateObject(card);
}

void MainModel::handleCardEditFinished(const QSharedPointer<API::Card>& card) {

    card->setOwnerSignature(getCurrentUser()->user()->getKey());

    auto localCard = _backEndModel->getCard(card->cardId());

    if (localCard && localCard->compare(*card.data())) {
        return;
    }

    auto listOfUsers = _backEndModel->getAllUserFromCard(card->cardId());

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

void MainModel::handleResetCardModel(const QSharedPointer<RC::API::Card> &card) {

    auto cardId = card->cardId();

    card->setCardVersion(0);
    _db->insertIfExistsUpdateObject(card);

    auto service = QmlNotificationService::NotificationService::getService();

    if (!_backEndModel->restoreOneCard(cardId)) {
        service->setNotify(tr("We seem to have a problems"),
                           tr("The card reset to default successful but load default card from server failed, "
                              "so you receive your card after new purchase in institution that has give out this card."),
                           "", QmlNotificationService::NotificationData::Warning);

        return;
    }

}

void MainModel::handleRemoveRequest(const QSharedPointer<API::Card> &card) {

    auto service = QmlNotificationService::NotificationService::getService();

    if (service) {

        QmlNotificationService::Listner listner = [card, this] (bool accepted) {

            if (accepted) {

                _db->deleteObject(card);
                _currentCardsListModel->removeCard(card->cardId());
            }
        };

        if (getMode()) {
            auto listOfUsers = _backEndModel->getAllUserFromCard(card->cardId());

            if (listOfUsers.size()) {
                service->setNotify(tr("Operation not permitted"),
                                tr("This card have a active clients, so you can't to remove this card."),
                                "",
                                QmlNotificationService::NotificationData::Error);
                return;
            }

            listner(true);
            return;
        }


        service->setQuestion(listner, tr("Remove Card"),
                             tr("You trying to delete this card, do not worry a seller that has give out this card save all bonuses locally, "
                                " so after repeat visit you will be get all your removed bonuses again."
                                " Do you want to continue?"));


    }
}

void MainModel::handleCardSelectedForWork(const QSharedPointer<CardModel> &card) {
    _waitModel->setCard(card);
}

void MainModel::handleCardSelectedForStatistic(const QSharedPointer<CardModel> &card) {
    auto usersList = _backEndModel->getAllUserFromCard(card->card()->cardId());
    auto usersDataList = _backEndModel->getAllUserDataFromCard(card->card()->cardId());

    _statisticModel->setDataList(card, usersList, usersDataList);
}

void MainModel::handleConnectWasBegin() {
    emit connectionWasBegin();
}

void MainModel::handleConnectWasFinished() {
    emit connectionWasEnd();
}

void MainModel::handlePurchaseWasSuccessful(QSharedPointer<RC::API::UsersCards> card, bool alert){

    soundEffectPlayback("Seal");
    auto cardModel = getCurrentListModel()->cache().value(card->getCard());
    int freeIndex = _backEndModel->getCardFreeIndex(card->getCard());

    if (_mode == Mode::Client) {
        getCurrentListModel()->updateMetaData({card});
    }

    int freeItems = _backEndModel->getFreeItemsCount(card, freeIndex);
    _db->insertIfExistsUpdateObject(card);

    if (freeItems > 0) {
        if (alert) {
            emit freeItem(cardModel.data(), card->getUser(), freeItems);
            soundEffectPlayback("Bonus");
        }

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

    auto header = QSharedPointer<API::UserHeader>::create();
    header->fromBytes(QByteArray::fromHex(extraData.toLatin1()));
    _lastUserHeader = header;

    sendSellerDataToServer(header, model->card()->cardId(), purchasesCount, false);
}

bool MainModel::sendSellerDataToServer(const QSharedPointer<API::UserHeader>& header,
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

void MainModel::handleSettingsChanged(const QString &key, const QVariant &) {
    if ("fFirst" == key) {
        emit fFirstChanged();
    }
}

QObject *MainModel::defaultLogosModel() const {
    return _defaultLogosModel;
}

QObject *MainModel::defaultBackgroundsModel() const {
    return _defaultBackgroundsModel;
}

QObject *MainModel::exportImportModel() const {
    return _importExportModel;
}

}
