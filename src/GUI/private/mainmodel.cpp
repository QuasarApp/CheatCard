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
#include "languagesmodel.h"
#include "activityprocessormodel.h"
#include "createcardmodel.h"
#include "iplatformtools.h"
#include "imagebackgroundsmodel.h"
#include "imagelogomodel.h"
#include "permisionsmodel.h"

#include <CheatCard/database.h>

#include "CheatCard/api/api0/user.h"
#include "CheatCard/api/api0/card.h"
#include "CheatCard/api/api0/userscards.h"
#include <CheatCard/api/api0/session.h>

#include <CheatCard/api/apiv1.h>
#include <CheatCard/api/apiv1-5.h>

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
#include <QQmlEngine>
#include "userslistmodel.h"

#include <CheatCardGui/ibilling.h>
#include "settingsmodel.h"

#include <doctorpillgui.h>

#include "CheatCard/pills/invalidcardidpill.h"

#include <qaglobalutils.h>



namespace RC {

void softRemove(BaseNode * obj) {
    obj->softDelete();
};

MainModel::MainModel(QH::ISqlDBCache *db) {
    _db = db;
    _soundEffect = new SoundPlayback;
    _config = dynamic_cast<SettingsModel*>(QuasarAppUtils::ISettings::instance());


    qRegisterMetaType<RC::API::UsersCards>();
    qRegisterMetaType<RC::API::Card>();
    qRegisterMetaType<RC::API::Session>();

    qRegisterMetaType<QSharedPointer<RC::API::UsersCards>>();
    qRegisterMetaType<QSharedPointer<RC::API::Card>>();
    qRegisterMetaType<QSharedPointer<RC::API::Session>>();
    qRegisterMetaType<QSharedPointer<RC::API::UserHeader>>();

    initBackgroundsModel();
    initIconsModel();
    initCardsListModels();
    initImagesModels();
    initWaitConnectionModel();
    initSellerStatisticModel();
    initImportExportModel();
    initNetIndicateModels();
    initDoctorModel();
    initLanguageModel();
    initActivityProcessorModel();
    initCreateCardModel();
    initPermisionsModel();

    configureCardsList();
    initUsersListModel();

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

    delete _statisticModel;
    delete _netIdicatorModel;

    delete _defaultLogosModel;
    delete _defaultBackgroundsModel;

    delete _waitModel;
    delete _soundEffect;
    delete _langModel;
    delete _activityProcessorModel;
    delete _createCardModel;
    delete _usersListModel;
    delete _backgrounds;
    delete _icons;
}

bool MainModel::fFirst() const {
    return _firstRun;
}

void MainModel::configureFinished() {
    // First run setiing id.
    saveUser();
    setFirst(false);

    _currentUser->regenerateSessionKey();
}

QObject *MainModel::getAboutModel() {
    if(!_aboutModel) {
        _aboutModel = new AboutModel();
        QQmlEngine::setObjectOwnership(_aboutModel, QQmlEngine::CppOwnership);

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

        service->setNotify(tr("We Have trouble"),
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

    auto newUser = _usersListModel->importUser(userData);
    _usersListModel->setCurrentUser(newUser->userId());
    saveUser();

    service->setNotify(tr("I managed to do it !"),
                       tr("Yor secret key are imported"),
                       "", QmlNotificationService::NotificationData::Normal);


    syncWithServer();
    return true;
}

void MainModel::handleCardCreated(const QSharedPointer<API::Card>& card) {
    auto list = getCurrentListModel();
    list->importCard(card);
    handleCardEditFinished(card);
}

void MainModel::handleAppOutdated(int) {
    _activityProcessorModel->newActivity("qrc:/CheatCardModule/UpdateRequestPage.qml");
}

void MainModel::handlePermissionChanged(const QSharedPointer<API::Contacts> &permision) {
    if (!_currentUser || !_currentUser->user()) {
        return;
    }

    _backEndModel->updateContactData(*permision,
                                     _currentUser->user()->secret(),
                                     false);
}

void MainModel::handlePermissionRemoved(QSharedPointer<API::Contacts> permision) {
    if (!_currentUser || !_currentUser->user()) {
        return;
    }

    _backEndModel->updateContactData(*permision,
                                     _currentUser->user()->secret(),
                                     true);
}

void MainModel::handlePermissionAdded(const QString &childUserName) {
    // send to server remove request

    if (!_currentUser || !_currentUser->user()) {
        return;
    }

    auto childUser = QSharedPointer<API::User>::create();
    auto contacts = QSharedPointer<API::Contacts>::create();

    if (!_backEndModel->createChilduser(childUserName, childUser, contacts)) {
        return;
    }

    _backEndModel->updateContactData(*contacts,
                                     _currentUser->user()->secret(),
                                     false);
}

void MainModel::handleContactsStatusResult(QSharedPointer<API::Contacts> contact,
                                           bool succesed, bool removed) {
    _permisionsModel->handleServerResult(contact,
                                         _currentUser->user(),
                                         succesed,
                                         removed);
}

const QSharedPointer<UserModel>& MainModel::getCurrentUser() const {
    return _currentUser;
}

void MainModel::setCurrentUser(const QSharedPointer<RC::UserModel>& value) {

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

        _permisionsModel->setPermissions(_backEndModel->getSlaveKeys(userKey),
                                         _currentUser->user());

        if (_billing) {
            connect(_currentUser.data(), &UserModel::sigBecomeSeller,
                    _billing, &IBilling::becomeSeller);
        }
    }

    emit currentUserChanged();
}

void MainModel::saveUser() {
    _db->insertIfExistsUpdateObject(_currentUser->user());
    _config->setCurrUser(_currentUser->user()->userId());
}

void MainModel::initCardsListModels() {
    _cardsListModel = new CardsListModel();
    _ownCardsListModel = new CardsListModel();

    _currentCardsListModel = new CardProxyModel();

    QQmlEngine::setObjectOwnership(_cardsListModel, QQmlEngine::CppOwnership);
    QQmlEngine::setObjectOwnership(_ownCardsListModel, QQmlEngine::CppOwnership);
    QQmlEngine::setObjectOwnership(_currentCardsListModel, QQmlEngine::CppOwnership);


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

    QQmlEngine::setObjectOwnership(_defaultLogosModel, QQmlEngine::CppOwnership);
    QQmlEngine::setObjectOwnership(_defaultBackgroundsModel, QQmlEngine::CppOwnership);

    _defaultLogosModel->setStringList(_icons->getImages());
    _defaultBackgroundsModel->setStringList(_backgrounds->getImages());
}

void MainModel::initNetIndicateModels() {
    _netIdicatorModel = new NetIndicatorModel();
    QQmlEngine::setObjectOwnership(_netIdicatorModel, QQmlEngine::CppOwnership);
}

void MainModel::initDoctorModel() {
    QList<QSharedPointer<DP::iPill>> pills;
    pills << QSharedPointer<InvalidCardIdPill>::create(_db);

    _doctorModel = new DP::DoctorModel(pills);
    QQmlEngine::setObjectOwnership(_doctorModel, QQmlEngine::CppOwnership);

}

void MainModel::initLanguageModel() {
    _langModel = new LanguagesModel();
    QQmlEngine::setObjectOwnership(_langModel, QQmlEngine::CppOwnership);

}

void MainModel::initActivityProcessorModel() {
    _activityProcessorModel = new ActivityProcessorModel();
    QQmlEngine::setObjectOwnership(_activityProcessorModel, QQmlEngine::CppOwnership);

}

void MainModel::initCreateCardModel() {
    _createCardModel = new CreateCardModel();
    QQmlEngine::setObjectOwnership(_createCardModel, QQmlEngine::CppOwnership);


    connect(_createCardModel, &CreateCardModel::sigCardCreated,
            this, &MainModel::handleCardCreated);
}

void MainModel::initUsersListModel() {
    _usersListModel = new UsersListModel(_icons);
    QQmlEngine::setObjectOwnership(_usersListModel, QQmlEngine::CppOwnership);

    auto request = QH::PKG::DBObjectsRequest<API::User>("Users");
    auto result = _db->getObject(request);

    _usersListModel->setUsers(result->data());
    setFirst(!result->data().size());

    connect(_usersListModel, &UsersListModel::sigUserChanged,
            this, &MainModel::setCurrentUser);

    if (!_usersListModel->rowCount()) {
        _usersListModel->importUser(QSharedPointer<API::User>::create());
    }

    _usersListModel->setCurrentUser(_config->getCurrUser());
}

void MainModel::initBackgroundsModel() {
    _backgrounds = new ImageBackgroundsModel();
}

void MainModel::initIconsModel() {
    _icons = new ImageLogoModel();
}

void MainModel::initPermisionsModel() {
    _permisionsModel = new PermisionsModel(_icons);

    QQmlEngine::setObjectOwnership(_permisionsModel, QQmlEngine::CppOwnership);

    connect(_permisionsModel, &PermisionsModel::sigPermisionUpdated,
            this, &MainModel::handlePermissionChanged);

    connect(_permisionsModel, &PermisionsModel::sigPermisionRemoved,
            this, &MainModel::handlePermissionRemoved);

    connect(_permisionsModel, &PermisionsModel::sigPermisionAdded,
            this, &MainModel::handlePermissionAdded);

}

void MainModel::setBackEndModel(const QSharedPointer<BaseNode>& newModel) {

    if (_backEndModel) {
        disconnect(_backEndModel.data(),
                   &BaseNode::sigAvailableNetworkChanged,
                   _netIdicatorModel,
                   &NetIndicatorModel::handleEndaleNetworkChanged);

        disconnect(_backEndModel.data(),
                   &BaseNode::sigDataExchangingChanged,
                   _netIdicatorModel,
                   &NetIndicatorModel::setDataExchanging);

        disconnect(_backEndModel.data(), &BaseNode::sigPurchaseWasSuccessful,
                   this, &MainModel::handlePurchaseWasSuccessful);

        disconnect(_backEndModel.data(), &BaseNode::sigCardReceived,
                   this, &MainModel::handleCardReceived);

        disconnect(_backEndModel.data(), &BaseNode::sigVersionNoLongerSupport,
                   this, &MainModel::handleAppOutdated);
        disconnect(_backEndModel.data(), &BaseNode::sigSessionStatusResult,
                   _waitModel, &WaitConnectionModel::handleSessionServerResult);

        disconnect(_backEndModel.data(), &BaseNode::sigContactsStatusResult,
                   this, &MainModel::handleContactsStatusResult);
    }

    _backEndModel = newModel;

    if (_backEndModel) {

        connect(_backEndModel.data(),
                &BaseNode::sigAvailableNetworkChanged,
                _netIdicatorModel,
                &NetIndicatorModel::handleEndaleNetworkChanged);

        connect(_backEndModel.data(),
                &BaseNode::sigDataExchangingChanged,
                _netIdicatorModel,
                &NetIndicatorModel::setDataExchanging);

        connect(_backEndModel.data(), &BaseNode::sigPurchaseWasSuccessful,
                this, &MainModel::handlePurchaseWasSuccessful);

        connect(_backEndModel.data(), &BaseNode::sigCardReceived,
                this, &MainModel::handleCardReceived);

        connect(_backEndModel.data(), &BaseNode::sigVersionNoLongerSupport,
                this, &MainModel::handleAppOutdated);

        connect(_backEndModel.data(), &BaseNode::sigSessionStatusResult,
                _waitModel, &WaitConnectionModel::handleSessionServerResult);

        connect(_backEndModel.data(), &BaseNode::sigContactsStatusResult,
                this, &MainModel::handleContactsStatusResult);


        _backEndModel->checkNetworkConnection();

    }
}

void MainModel::initWaitConnectionModel() {
    _waitModel = new WaitConnectionModel();
    QQmlEngine::setObjectOwnership(_waitModel, QQmlEngine::CppOwnership);

    connect(_waitModel, &WaitConnectionModel::purchaseTaskCompleted,
            this, &MainModel::handleListenStart, Qt::QueuedConnection);
}

void MainModel::initSellerStatisticModel() {
    _statisticModel = new SellerStatisticModel;
    QQmlEngine::setObjectOwnership(_statisticModel, QQmlEngine::CppOwnership);

}

void MainModel::initImportExportModel() {
    if (!_importExportModel) {
        _importExportModel = new ImportExportUserModel();
        QQmlEngine::setObjectOwnership(_importExportModel, QQmlEngine::CppOwnership);

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
    bool fSallerEnabled = _config && _config->getValue(P_FSELLER, false).toBool();
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

bool MainModel::fBillingAwailable() const {
    return _billing && _billing->isSupported();
}

QString MainModel::storeLink() const {
    auto platformsTools = IPlatformTools::instance();
    if (platformsTools) {
        return platformsTools->storeLink();
    }

    return "";
}

void MainModel::reload() const {
    if (_netIdicatorModel->dataExchanging()) {
        return;
    }

    syncWithServer();
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

    result->addApiParser<ApiV1_5>();

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

void RC::MainModel::syncWithServer() const{
    auto service = QmlNotificationService::NotificationService::getService();

    if (!_backEndModel->restoreAllData(_currentUser->user()->getKey())) {
        service->setNotify(tr("We Have trouble"),
                           tr("Failed to sync data with server."
                              " Please check your internet connection and try to restore your data again"),
                           "", QmlNotificationService::NotificationData::Warning);
    }
}

void MainModel::setFirst(bool ffirst) {
    if (_firstRun == ffirst) {
        return;
    }

    _firstRun = ffirst;
    emit fFirstChanged();
}

void MainModel::setMode(int newMode) {
    newMode = newMode && fBillingAwailable();

    if (static_cast<int>(_mode) == newMode)
        return;

    _mode = static_cast<Mode>(newMode);
    emit modeChanged();

    configureCardsList();

    _config->setValue(P_FSELLER, static_cast<bool>(newMode));

    syncWithServer();
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
    bool fClient = getMode() == static_cast<int>(Mode::Client);
    if (fClient) {
        card->setCardVersion(VERSION_CARD_USER);
    } else {
        card->setCardVersion(card->getCardVersion() + 1);

    }

    _db->insertIfExistsUpdateObject(card);

    // send notification about updates to server
    if (!fClient) {
        auto seller = _backEndModel.staticCast<Seller>();
        seller->cardUpdated(card->cardId(), card->getCardVersion());
    }
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
                                 tr(" You're trying to change the bonus rules."
                                    " These changes will be saved as a new card."
                                    " The old card continues to work correctly and all customers data will be saved."
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
        service->setNotify(tr("We seem to have problems"),
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
            auto listOfUsers = _backEndModel->getAllActiveUserFromCard(card->cardId());

            if (listOfUsers.size()) {
                service->setNotify(tr("Operation not permitted"),
                                   tr("This card has active clients, so you can't to remove this card."),
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
    _activityProcessorModel->newActivity("qrc:/CheatCardModule/WaitConnectView.qml",
                                         _waitModel);
}

void MainModel::handleCardSelectedForStatistic(const QSharedPointer<CardModel> &card) {
    auto usersList = _backEndModel->getAllUserFromCard(card->card()->cardId());
    auto usersDataList = _backEndModel->getAllUserDataFromCard(card->card()->cardId());

    _statisticModel->setDataList(card, usersList, usersDataList);

    _activityProcessorModel->newActivity("qrc:/CheatCardModule/SellerStatistic.qml",
                                         _statisticModel);
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
            if (!_activityProcessorModel->freeItem(cardModel.data(), card->getUser(), freeItems)) {
                QuasarAppUtils::Params::log("Fail to show bonus page.",
                                            QuasarAppUtils::Error);
            }
            soundEffectPlayback("Bonus");
        }

    } else {
        if (_mode == Mode::Seller && _fShowEmptyBonuspackaMessage && alert) {
            _fShowEmptyBonuspackaMessage = false;

            auto service = QmlNotificationService::NotificationService::getService();

            service->setNotify(tr("Sorry but not"),
                               tr("This client does not have any bonuses. Sorry... "),
                               "", QmlNotificationService::NotificationData::Normal);

        }
    }
}

void MainModel::handleListenStart(int purchasesCount,
                                  QSharedPointer<RC::CardModel> model,
                                  QSharedPointer<API::UserHeader> header) {

    _lastUserHeader = header;

    sendSellerDataToServer(header, model->card()->cardId(), purchasesCount, false);
}

bool MainModel::sendSellerDataToServer(const QSharedPointer<API::UserHeader>& header,
                                       unsigned int cardId,
                                       int purchasesCount,
                                       bool receive) {

    auto seller = _backEndModel.dynamicCast<Seller>();

    if (!seller)
        return false;

    bool sendResult = false;
    if (receive) {
        sendResult = seller->sentDataToServerReceive(header, cardId, purchasesCount);
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

QObject *MainModel::activityProcessorModel() const {
    return _activityProcessorModel;
}

QObject *MainModel::doctorModel() const {
    return _doctorModel;
}

QObject *MainModel::langModel() const {
    return _langModel;
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

    debug_assert(static_cast<unsigned int>(userId) == _lastUserHeader->getUserId(),
                 "handleBonusGivOut function should be works with one user!");

    sendSellerDataToServer(_lastUserHeader, cardId, count, true);

}

void MainModel::handleSettingsChanged(const QString &, const QVariant &) {
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

QObject *MainModel::createCardModel() const {
    return _createCardModel;
}

QObject *MainModel::usersListModel() const {
    return _usersListModel;
}

QObject *MainModel::permisionsModel() const {
    return _permisionsModel;
}

}
