//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "CheatCard/basenode.h"
#include "CheatCard/userheader.h"
#include "models/cardmodel.h"
#include "models/doctorpillmodel.h"
#include "models/netindicatormodel.h"
#include "models/aboutmodel.h"
#include "models/itemsmodel.h"
#include "mainmodel.h"
#include "rcdb/settingskeys.h"
#include "models/waitconnectionmodel.h"
#include "soundplayback.h"
#include "models/cardproxymodel.h"
#include "models/sellerstatisticmodel.h"
#include "models/importexportusermodel.h"
#include "models/languagesmodel.h"
#include "models/activityprocessormodel.h"
#include "models/createcardmodel.h"
#include "iplatformtools.h"
#include "models/imagebackgroundsmodel.h"
#include "models/imagelogomodel.h"
#include "models/permisionsmodel.h"

#include "models/cardslistmodel.h"
#include "models/usermodel.h"
#include <getsinglevalue.h>

#include <QCoreApplication>
#include "cmath"
#include <qmlnotifyservice.h>
#include <QDir>
#include <QGuiApplication>
#include "models/userslistmodel.h"
#include <CheatCardGui/ibilling.h>
#include "models/settingsmodel.h"
#include <doctorpillgui.h>
#include <qaglobalutils.h>
#include <CheatCard/client.h>
#include <CheatCard/usersnames.h>
#include <api/apibase.h>
#include <api.h>


namespace RC {

void softRemove(BaseNode * obj) {
    obj->softDelete();
};

MainModel::MainModel(const QSharedPointer<Interfaces::iDB> &db) {
    _db = db;
    _soundEffect = new SoundPlayback;
    _config = dynamic_cast<SettingsModel*>(QuasarAppUtils::ISettings::instance());
    _modelStorage = QSharedPointer<ModelsStorage>::create(_db);

    qRegisterMetaType<QSharedPointer<RC::CardModel>>();
    qRegisterMetaType<QSharedPointer<RC::Interfaces::iUsersCards>>();
    qRegisterMetaType<QSharedPointer<RC::UserHeader>>();
    qRegisterMetaType<QSharedPointer<RC::Interfaces::iSession>>();
    qRegisterMetaType<QSharedPointer<RC::Interfaces::iContacts>>();
    qRegisterMetaType<QSharedPointer<RC::Interfaces::iCard>>();

    initModels();

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
    delete _soundEffect;

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
    return _modelStorage->initAndGetRaw<AboutModel>(DEFAULT_CALL_BACK);
}

QObject *MainModel::getNetIndicatorModel() const {
    return _modelStorage->getRaw<NetIndicatorModel>();
}

QObject *MainModel::currentUser() const {
    return _currentUser.data();
}

bool MainModel::handleImportUser(const QString &base64UserData) {

    auto service = QmlNotificationService::NotificationService::getService();

    auto secret = QByteArray::fromBase64(base64UserData.toLatin1(),
                                         QByteArray::Base64UrlEncoding);

    if (secret.size() != 32) {
        service->setNotify(tr("Your reserve QR code is deprecated"),
                           tr("It is still works, but in the next releases the support of this qr code will be dropped."
                              " Please. Create the new recovery Qr code."),
                           "", QmlNotificationService::NotificationData::Warning);

        secret = importDeprecatedUser(base64UserData);
    }

    auto userData = _db->makeEmptyUser();
    userData->regenerateKeys(secret);

    if (!userData->isValid()) {
        if (service) {

            service->setNotify(tr("Wow shit"),
                               tr("This qr code is invalid. Sorry... "),
                               "", QmlNotificationService::NotificationData::Error);
        }

        return false;
    }

    auto usersListModel = _modelStorage->get<UsersListModel>();

    auto newUser = usersListModel->importUser(userData);
    usersListModel->setCurrentUser(newUser->userId());
    saveUser();

    service->setNotify(tr("I managed to do it !"),
                       tr("Yor secret key are imported"),
                       "", QmlNotificationService::NotificationData::Normal);


    syncWithServer();
    return true;
}

QByteArray MainModel::importDeprecatedUser(const QString &base64UserData) {

    auto fromStream = [](QDataStream &stream) {

        QVariant plug;
        stream >> plug;

        QString plugString;
        QByteArray plugBytes, secreet;
        stream >> plugString;
        stream >> plugBytes;
        stream >> secreet;

        return secreet;
    };

    QByteArray data = QByteArray::fromBase64(base64UserData.toLatin1(),
                                             QByteArray::Base64UrlEncoding);
    if (data.isEmpty())
        return {};

    QDataStream stream(data);
    return fromStream(stream);

}

void MainModel::handleCardCreated(const QSharedPointer<Interfaces::iCard>& card) {
    auto list = getCurrentListModel();
    list->importCard(card);
    handleCardEditFinished(card);
}

void MainModel::handleAppOutdated(int) {
    auto model = _modelStorage->get<ActivityProcessorModel>();
    model->newActivity("qrc:/CheatCardModule/UpdateRequestPage.qml");
}

void MainModel::handlePermissionChanged(const QSharedPointer<Interfaces::iContacts> &permision) {
    if (!_currentUser || !_currentUser->user()) {
        return;
    }

    _backEndModel->updateContactData(permision,
                                     _currentUser->user()->secret(),
                                     false);
}

void MainModel::handlePermissionRemoved(QSharedPointer<Interfaces::iContacts> permision) {
    if (!_currentUser || !_currentUser->user()) {
        return;
    }

    _backEndModel->updateContactData(permision,
                                     _currentUser->user()->secret(),
                                     true);
}

void MainModel::handlePermissionAdded(QSharedPointer<UserHeader> childUserName) {
    // send to server remove request

    if (!_currentUser || !_currentUser->user()) {
        return;
    }

    auto childUser = _db->makeEmptyUser();
    childUserName->toUser(childUser);

    childUser->setName(childUserName->userName());
    if (childUser->name().isEmpty()) {
        childUser->setName(UsersNames::randomUserName());
    }

    auto contacts = _db->makeEmptyContact();
    if (!_backEndModel->createContact(childUser, contacts)) {
        return;
    }

    _backEndModel->updateContactData(contacts,
                                     _currentUser->user()->secret(),
                                     false);
}

void MainModel::handleContactsListChanged() {
    QByteArray userKey = _currentUser->user()->getKey();

    auto masterKeys = _db->getMasterKeys(userKey);
    _ownCardsListModel->setCards(_db->getAllUserCards(userKey,
                                                      false,
                                                      masterKeys));


}

void MainModel::handleSerrverSentError(unsigned char code,
                                       QString errorMessage) {

    auto service = QmlNotificationService::NotificationService::getService();
    service->setNotify(tr("We Have trouble: trouble code is %0").arg(static_cast<int>(code)),
                       tr("Server sent the error message."
                          " Message: \"%0.\" "
                          " Sorry ;)").arg(errorMessage),
                       "", QmlNotificationService::NotificationData::Error);
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

        unsigned int userId = _currentUser->user()->id();
        QByteArray userKey = _currentUser->user()->getKey();

        _config->setCurrUser(userId);

        _currentUser->regenerateSessionKey();

        // get list of owned cards
        auto masterKeys = _db->getMasterKeys(userKey);
        _ownCardsListModel->setCards(_db->getAllUserCards(userKey,
                                                          false,
                                                          masterKeys));

        // get list of included cards
        _cardsListModel->setCards(_db->getAllUserCards(userKey,
                                                       true,
                                                       masterKeys));

        _cardsListModel->updateMetaData(_db->getAllUserData(userId));
        auto model = _modelStorage->get<PermisionsModel>();
        model->setPermissions(_db->getSlaveKeys(userKey));

        if (_billing) {
            connect(_currentUser.data(), &UserModel::sigBecomeSeller,
                    _billing, &IBilling::becomeSeller);
            _billing->init();
        }
    }

    emit currentUserChanged();
}

void MainModel::saveUser() {

    if (_currentUser->user()->secret().isEmpty())
        return;

    _db->saveUser(_currentUser->user());
    _config->setCurrUser(_currentUser->user()->id());
}

void MainModel::initCardsListModels() {

    _cardsListModel = _modelStorage->init<CardsListModel>();
    _ownCardsListModel = _modelStorage->init<CardsListModel>();

    _currentCardsListModel = _modelStorage->init<CardProxyModel>();

    _currentCardsListModel->sort(0);
    _currentCardsListModel->setDynamicSortFilter(true);
    _currentCardsListModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    connect(_cardsListModel.data(), &CardsListModel::sigRemoveRequest,
            this, &MainModel::handleRemoveRequest);

    connect(_cardsListModel.data(), &CardsListModel::sigEditFinished,
            this, &MainModel::saveCard);

    connect(_cardsListModel.data(), &CardsListModel::sigResetCardModel,
            this, &MainModel::handleResetCardModel);

    connect(_ownCardsListModel.data(), &CardsListModel::sigEditFinished,
            this, &MainModel::handleCardEditFinished);

    connect(_ownCardsListModel.data(), &CardsListModel::sigRemoveRequest,
            this, &MainModel::handleRemoveRequest);

    connect(_ownCardsListModel.data(), &CardsListModel::sigCardSelectedForWork,
            this, &MainModel::handleCardSelectedForWork);

    connect(_ownCardsListModel.data(), &CardsListModel::sigCardSelectedForStatistic,
            this, &MainModel::handleCardSelectedForStatistic);

    connect(_ownCardsListModel.data(), &CardsListModel::sigResetCardModel,
            this, &MainModel::handleResetCardModel);
}

void MainModel::initImagesModels() {
    _defaultLogosModel = _modelStorage->init<ItemsModel>();
    _defaultBackgroundsModel = _modelStorage->init<ItemsModel>();

    _defaultLogosModel->setStringList(_modelStorage->get<ImageLogoModel>()->getImages());
    _defaultBackgroundsModel->setStringList(_modelStorage->get<ImageBackgroundsModel>()->getImages());
}

void MainModel::setBackEndModel(const QSharedPointer<BaseNode>& newModel) {

    auto netIdicatorModel = _modelStorage->getRaw<NetIndicatorModel>();
    auto permissionsModel = _modelStorage->getRaw<PermisionsModel>();
    auto waitModel = _modelStorage->getRaw<WaitConnectionModel>();

    if (_backEndModel) {
        disconnect(_backEndModel.data(),
                   &BaseNode::sigAvailableNetworkChanged,
                   netIdicatorModel,
                   &NetIndicatorModel::handleEndaleNetworkChanged);

        disconnect(_backEndModel.data(),
                   &BaseNode::sigDataExchangingChanged,
                   netIdicatorModel,
                   &NetIndicatorModel::setDataExchanging);

        disconnect(_backEndModel.data(), &BaseNode::sigPurchaseWasSuccessful,
                   this, &MainModel::handlePurchaseWasSuccessful);

        disconnect(_backEndModel.data(), &BaseNode::sigCardReceived,
                   this, &MainModel::handleCardReceived);

        disconnect(_backEndModel.data(), &BaseNode::sigVersionNoLongerSupport,
                   this, &MainModel::handleAppOutdated);

        disconnect(_backEndModel.data(), &BaseNode::sigSessionStatusResult,
                   waitModel, &WaitConnectionModel::handleSessionServerResult);

        disconnect(_backEndModel.data(), &BaseNode::sigContactsStatusResult,
                   permissionsModel, &PermisionsModel::handleServerResult);

        disconnect(_backEndModel.data(), &BaseNode::sigContactsListChanged,
                   this, &MainModel::handleContactsListChanged);

        disconnect(_backEndModel.data(), &BaseNode::requestError,
                   this, &MainModel::handleSerrverSentError);


    }

    _backEndModel = newModel;

    if (_backEndModel) {

        connect(_backEndModel.data(),
                &BaseNode::sigAvailableNetworkChanged,
                netIdicatorModel,
                &NetIndicatorModel::handleEndaleNetworkChanged);

        connect(_backEndModel.data(),
                &BaseNode::sigDataExchangingChanged,
                netIdicatorModel,
                &NetIndicatorModel::setDataExchanging);

        connect(_backEndModel.data(), &BaseNode::sigPurchaseWasSuccessful,
                this, &MainModel::handlePurchaseWasSuccessful);

        connect(_backEndModel.data(), &BaseNode::sigCardReceived,
                this, &MainModel::handleCardReceived);

        connect(_backEndModel.data(), &BaseNode::sigVersionNoLongerSupport,
                this, &MainModel::handleAppOutdated);

        connect(_backEndModel.data(), &BaseNode::sigSessionStatusResult,
                waitModel, &WaitConnectionModel::handleSessionServerResult);

        connect(_backEndModel.data(), &BaseNode::sigContactsStatusResult,
                permissionsModel, &PermisionsModel::handleServerResult);

        connect(_backEndModel.data(), &BaseNode::sigContactsListChanged,
                this, &MainModel::handleContactsListChanged);

        connect(_backEndModel.data(), &BaseNode::requestError,
                this, &MainModel::handleSerrverSentError);

        _backEndModel->checkNetworkConnection();

    }
}

void MainModel::setCardListModel(const QSharedPointer<CardsListModel>& model) {
    if (_currentCardsListModel->sourceModel() == model.data())
        return;

    _currentCardsListModel->setSourceModel(model.data());
}

void MainModel::initMode(const QSharedPointer<UserModel> &user) {
    bool fSallerEnabled = _config && _config->getValue(P_FSELLER, false).toBool();
    setMode(user && user->fSaller() && fSallerEnabled);
}

void MainModel::soundEffectPlayback(const QString &soundName) {
    _soundEffect->playSound(soundName);
}

const QSharedPointer<Interfaces::iDB> &MainModel::db() const {
    return _db;
}

void MainModel::initModels() {
    _modelStorage->add<ImageBackgroundsModel>(DEFAULT_CALL_BACK);
    _modelStorage->add<ImageLogoModel>(DEFAULT_CALL_BACK);

    initCardsListModels();
    initImagesModels();

    _modelStorage->add<WaitConnectionModel>([this](auto model, auto){

        connect(model.data(), &WaitConnectionModel::purchaseTaskCompleted,
                this, &MainModel::handleListenStart, Qt::QueuedConnection);

        return true;
    });

    _modelStorage->add<SellerStatisticModel>(DEFAULT_CALL_BACK);

    _modelStorage->add<ImportExportUserModel>([this](auto mdoel, auto){
        emit exportImportModelChanged();

        connect(mdoel.data(), &ImportExportUserModel::decodeFinished,
                this, &MainModel::handleImportUser);
        return true;
    });

    _modelStorage->add<NetIndicatorModel>(DEFAULT_CALL_BACK); //initNetIndicateModels();
    _modelStorage->add<DoctorPillModel>([](auto, auto){return true;},
                                        _db->initPills());
    _modelStorage->add<LanguagesModel>(DEFAULT_CALL_BACK);
    _modelStorage->add<ActivityProcessorModel>(DEFAULT_CALL_BACK);

    _modelStorage->add<CreateCardModel>([this](auto model, auto){

        connect(model.data(), &CreateCardModel::sigCardCreated,
                this, &MainModel::handleCardCreated);

        return true;
    });

    _modelStorage->add<PermisionsModel>([this](auto model, auto ) {

        connect(model.data(), &PermisionsModel::sigPermisionUpdated,
                this, &MainModel::handlePermissionChanged);

        connect(model.data(), &PermisionsModel::sigPermisionRemoved,
                this, &MainModel::handlePermissionRemoved);

        connect(model.data(), &PermisionsModel::sigPermisionAdded,
                this, &MainModel::handlePermissionAdded);
        return true;
    });

    configureCardsList();

    _modelStorage->add<UsersListModel>([this](auto model, auto) {

        auto result = _db->getAllUserWithPrivateKeys();

        model->setUsers(result);
        setFirst(!result.size());

        connect(model.data(), &UsersListModel::sigUserChanged,
                this, &MainModel::setCurrentUser);

        if (!model->rowCount()) {
            model->importUser(_db->makeEmptyUser());
        }

        model->setCurrentUser(_config->getCurrUser());

        return true;

    });
}

QObject *MainModel::waitModel() const {
    return _modelStorage->getRaw<WaitConnectionModel>();
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

    if (!_db)
        return 0;

    return _db->getCountOfReceivedItems(
        _currentUser->user()->id(),
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
    auto netIdicatorModel = _modelStorage->getRaw<NetIndicatorModel>();
    if (netIdicatorModel->dataExchanging()) {
        return;
    }

    syncWithServer();
}

QObject *MainModel::statisticModel() const {
    return _modelStorage->getRaw<SellerStatisticModel>();
}

int MainModel::getMode() const {
    return static_cast<int>(_mode);
}

// template method for initialize back end model. using in configureCardsList method
template <class BackEndType>
QSharedPointer<BaseNode> initBackEndModel(const QSharedPointer<UserModel>& user,
                                          const QSharedPointer<Interfaces::iDB> &db) {
    QSharedPointer<BaseNode> result;
    result = QSharedPointer<BaseNode>(new BackEndType(db), softRemove);

//    const auto apis = API::init({2}, db, result.data());
//    if (user) {
//        result->setCurrentUser(user->user());
//    }

//    for (const auto & api: apis) {
//        MainModel::connect(api.data(), &API::APIBase::sigCardReceived,
//                           result.data(), &BaseNode::sigCardReceived, Qt::DirectConnection);

//        MainModel::connect(api.data(), &API::APIBase::sigContactsStatusResult,
//                           result.data(), &BaseNode::sigContactsStatusResult, Qt::DirectConnection);

//        MainModel::connect(api.data(), &API::APIBase::sigSessionStatusResult,
//                           result.data(), &BaseNode::sigSessionStatusResult, Qt::DirectConnection);

//        MainModel::connect(api.data(), &API::APIBase::sigContactsListChanged,
//                           result.data(), &BaseNode::sigContactsListChanged, Qt::DirectConnection);

//        MainModel::connect(api.data(), &API::APIBase::sigPurchaseWasSuccessful,
//                           result.data(), &BaseNode::sigPurchaseWasSuccessful, Qt::DirectConnection);

//    }


    return result;
};

void RC::MainModel::configureCardsList() {
    if (_mode == Mode::Client) {
        setCardListModel(_cardsListModel);
        if (!_visitorbackEndModel) {
            _visitorbackEndModel = initBackEndModel<RC::Visitor>(_currentUser,
                                                                 _db);
        }

        setBackEndModel(_visitorbackEndModel);
    } else {
        setCardListModel(_ownCardsListModel);
        if (!_sellerbackEndModel) {
            _sellerbackEndModel = initBackEndModel<RC::Seller>(_currentUser,
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
    return _currentCardsListModel.data();
}

void MainModel::handleCardReceived(QSharedPointer<RC::Interfaces::iCard> card) {

    if (card->isOvner(_currentUser->user()->id())) {
        _ownCardsListModel->importCard(card);

    } else {
        if (!_cardsListModel)
            return;

        _cardsListModel->importCard(card);
        if (_db) {
            auto metaData = _db->getUserCardData(_currentUser->user()->id(), card->cardId());

            if (metaData) {
                _cardsListModel->updateMetaData({metaData});
            }
        }
    }
}

void RC::MainModel::saveCard(const QSharedPointer<Interfaces::iCard>& card) {
    bool fClient = getMode() == static_cast<int>(Mode::Client);
    if (fClient) {
        card->setCardVersion(VERSION_CARD_USER);
    } else {
        card->setCardVersion(card->getCardVersion() + 1);
    }

    _db->saveCard(card);

    // send notification about updates to server
    auto currentUserId  = _currentUser->userId();
    if (card->isOvner(currentUserId)) {

        auto seller = _backEndModel.dynamicCast<Seller>();
        if (!seller)
            return;

        auto header = _currentUser->getHelloPackage();

        seller->setPurchase(header,
                            card->cardId(),
                            0);

    }
}

void MainModel::handleCardEditFinished(const QSharedPointer<Interfaces::iCard>& card) {

    card->setOwnerSignature(getCurrentUser()->user()->getKey());

    auto localCard = _db->getCard(card->cardId());

    if (localCard && localCard->compare(card)) {
        return;
    }

    auto listOfUsers = _db->getAllUserFromCard(card->cardId(),
                                               _currentUser->userId());

    if (localCard && listOfUsers.size() && localCard->getFreeIndex() != card->getFreeIndex()) {

        auto service = QmlNotificationService::NotificationService::getService();

        if (service) {

            QmlNotificationService::Listner listner = [card, localCard, this] (bool accepted) {
                getCurrentListModel()->importCard(localCard);
                saveCard(localCard);

                if (accepted) {

                    card->setId(rand());
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

void MainModel::handleResetCardModel(const QSharedPointer<Interfaces::iCard> &card) {

    auto cardId = card->cardId();

    card->setCardVersion(0);
    _db->saveCard(card);

    auto service = QmlNotificationService::NotificationService::getService();

    if (!_backEndModel->restoreOneCard(cardId)) {
        service->setNotify(tr("We seem to have problems"),
                           tr("The card reset to default successful but load default card from server failed, "
                              "so you receive your card after new purchase in institution that has give out this card."),
                           "", QmlNotificationService::NotificationData::Warning);

        return;
    }
}

void MainModel::handleRemoveRequest(const QSharedPointer<Interfaces::iCard> &card) {

    auto service = QmlNotificationService::NotificationService::getService();

    if (service) {

        QmlNotificationService::Listner listner = [card, this] (bool accepted) {

            if (accepted) {
                _db->deleteCard(card->cardId());
                _currentCardsListModel->removeCard(card->cardId());
            }
        };

        if (getMode()) {
            auto listOfUsers = _db->getAllActiveUserFromCard(card->cardId(),
                                                             Interfaces::ACTIVE_USER_TIME_LIMIT,
                                                             _currentUser->userId());

            if (listOfUsers.size()) {
                service->setNotify(tr("Operation not permitted"),
                                   tr("This card has active clients, so you can't to remove this card."),
                                   "",
                                   QmlNotificationService::NotificationData::Error);
                return;
            }

            listner(true);

            // only owners can remove the card on server
            if (card->isOvner(_currentUser->userId()) ) {
                if (auto backEnd = _backEndModel.dynamicCast<Seller>())
                    backEnd->deleteCard(card->cardId());
            }
            return;
        }


        service->setQuestion(listner, tr("Remove Card"),
                             tr("You trying to delete this card, do not worry a seller that has give out this card save all bonuses locally, "
                                " so after repeat visit you will be get all your removed bonuses again."
                                " Do you want to continue?"));


    }
}

void MainModel::handleCardSelectedForWork(const QSharedPointer<CardModel> &card) {
    if (auto waitModel = _modelStorage->get<WaitConnectionModel>()) {
        waitModel->setCard(card);

        if (auto activityModel = _modelStorage->get<ActivityProcessorModel>()) {
            activityModel->newActivity("qrc:/CheatCardModule/WaitConnectView.qml",
                                       waitModel.data());
        }
    }
}

void MainModel::handleCardSelectedForStatistic(const QSharedPointer<CardModel> &card) {

    if (auto statisticModel = _modelStorage->get<SellerStatisticModel>()) {
        auto usersList = _db->getAllUserFromCard(card->card()->cardId(), _currentUser->userId());
        auto usersDataList = _db->getAllUserDataFromCard(card->card()->cardId());

        statisticModel->setDataList(card, usersList, usersDataList);

        if (auto activityModel = _modelStorage->get<ActivityProcessorModel>()) {
            activityModel->newActivity("qrc:/CheatCardModule/SellerStatistic.qml",
                                       statisticModel.data());
        }
    }
}

void MainModel::handlePurchaseWasSuccessful(QSharedPointer<RC::Interfaces::iUsersCards> card, bool alert){

    if (alert)
        soundEffectPlayback("Seal");

    auto cardModel = getCurrentListModel()->cache().value(card->getCard());

    if (_mode == Mode::Client) {
        getCurrentListModel()->updateMetaData({card});
    }

    int freeItems = _db->getFreeItemsCount(card);
    _db->saveUsersCard(card);

    if (freeItems > 0) {
        if (alert) {
            auto apModel = _modelStorage->get<ActivityProcessorModel>();
            if (!(apModel && apModel->freeItem(cardModel.data(), card->getUser(), freeItems))) {
                QuasarAppUtils::Params::log("Fail to show bonus page.",
                                            QuasarAppUtils::Error);
            }

            if (alert)
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
                                  QSharedPointer<UserHeader> header) {

    _lastUserHeader = header;

    QSharedPointer<Interfaces::iUser> client = _db->makeEmptyUser();
    if (!header->toUser(client) || !client->isValid()) {
        auto service = QmlNotificationService::NotificationService::getService();

        service->setNotify(tr("Oops"),
                           tr("Some kind of garbage happened when reading the qr code. Try again"),
                           "", QmlNotificationService::NotificationData::Warning);

        return;
    }

    auto dbClientData = _db->getUser(header->getUserId());
    if (client->name().isEmpty()) {
        if (!dbClientData || dbClientData->name().isEmpty()) {
            client->setName(UsersNames::randomUserName());
        } else {
            client->setName(dbClientData->name());
        }
    }

    // update only external users data.
    if (!dbClientData || dbClientData->secret().isEmpty()) {
        db()->saveUser(client);
    }

    sendSellerDataToServer(header, model->card()->cardId(), purchasesCount, false);
}

bool MainModel::sendSellerDataToServer(const QSharedPointer<UserHeader> &header,
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
    return _modelStorage->getRaw<ActivityProcessorModel>();
}

QObject *MainModel::doctorModel() const {
    return _modelStorage->getRaw<DoctorPillModel>();
}

QObject *MainModel::langModel() const {
    return _modelStorage->getRaw<LanguagesModel>();
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

    if (!(visitor && _currentUser)) {
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
    return _defaultLogosModel.data();
}

QObject *MainModel::defaultBackgroundsModel() const {
    return _defaultBackgroundsModel.data();
}

QObject *MainModel::exportImportModel() const {
    return _modelStorage->getRaw<ImportExportUserModel>();

}

QObject *MainModel::createCardModel() const {
    return _modelStorage->getRaw<CreateCardModel>();
}

QObject *MainModel::usersListModel() const {
    return _modelStorage->getRaw<UsersListModel>();

}

QObject *MainModel::permisionsModel() const {
    return _modelStorage->getRaw<PermisionsModel>();
}

}
