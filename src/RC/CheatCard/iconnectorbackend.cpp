//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "iconnectorbackend.h"
#include "quasarapp.h"
#include "user.h"
#include "card.h"
#include "datastructures.h"
#include "itargetnode.h"
#include "userscards.h"
#include "dbobjectsrequest.h"
#include "getsinglevalue.h"
#include <cstring>
#include <qmlnotifyservice.h>
#include <session.h>
#include <QBuffer>
#include "dbobjectsrequest.h"

#define TIME_LIMIT_SEC 10

namespace RC {


IConnectorBackEnd::IConnectorBackEnd(DB *db) {
    _db = db;
}

IConnectorBackEnd::~IConnectorBackEnd() {
    if(_lastReceivedCardStatus)
        delete _lastReceivedCardStatus;
}

bool IConnectorBackEnd::start(Mode mode) {
    return listen(mode);
}

bool IConnectorBackEnd::stop() {
    return close();
}

void IConnectorBackEnd::reset() {

    close();

    _currentTarget.reset();
}

void IConnectorBackEnd::connectionReceived(ITargetNode *obj) {

    if (_currentTarget) {
        disconnect(_currentTarget.data(), &ITargetNode::sigMessageReceived,
                this, &IConnectorBackEnd::handleReceiveMessage);

        disconnect(_currentTarget.data(), &ITargetNode::sigConnectionClosed,
                this, &IConnectorBackEnd::handleConnectionClosed);
    }

    _currentTarget = QSharedPointer<ITargetNode>(obj);

    if (_currentTarget) {
        connect(_currentTarget.data(), &ITargetNode::sigMessageReceived,
                this, &IConnectorBackEnd::handleReceiveMessage, Qt::QueuedConnection);
        connect(_currentTarget.data(), &ITargetNode::sigConnectionClosed,
                this, &IConnectorBackEnd::handleConnectionClosed, Qt::QueuedConnection);

    }

    beginWork();
}

void IConnectorBackEnd::connectionLost(unsigned int nodeID) {
    if (_currentTarget->nodeId() != nodeID) {
        QuasarAppUtils::Params::log("Try drop another connection!!", QuasarAppUtils::Error);
    }

    endWork(ConnectionLost);
}

int IConnectorBackEnd::getPurchasesCount(unsigned int userId,
                                         unsigned int cardId) {

    auto purches = getUserCardData(userId, cardId);

    if (purches) {
        return purches->getPurchasesNumber();
    }

    return 0;
}

QSharedPointer<UsersCards>
IConnectorBackEnd::getUserCardData(unsigned int userId, unsigned int cardId) {
    UsersCards request;
    request.setCard(cardId);
    request.setUser(userId);

    auto purches = _db->getObject(request);
    return purches;
}

void IConnectorBackEnd::handleReceiveMessage(QByteArray message) {

    unsigned char command = *message.begin();

    switch (command) {
    case UserId : {
        if (!processUserRequest(message)) {
            QuasarAppUtils::Params::log("Failed to parse package (UserId)", QuasarAppUtils::Error);
            endWork(WrongPackage);
        }
        break;

    }

    case StatusRequest: {
        if (!processStatusRequest(message)) {
            QuasarAppUtils::Params::log("Failed to parse package (processStatusRequest)", QuasarAppUtils::Error);
            endWork(WrongPackage);

        }

        break;
    }

    case StatusResponce: {
        if (!processCardStatus(message)) {
            QuasarAppUtils::Params::log("Failed to parse package (StatusResponce)", QuasarAppUtils::Error);
            endWork(WrongPackage);

        }

        break;
    }

    case CardDataRequest: {
        if (!processCardRequest(message)) {
            QuasarAppUtils::Params::log("Failed to parse package (CardDataRequest)", QuasarAppUtils::Error);
            endWork(WrongPackage);

        }
        break;
    }

    case CardData: {
        if (!processCardData(message)) {
            QuasarAppUtils::Params::log("Failed to parse package (processCardData)", QuasarAppUtils::Error);
            endWork(WrongPackage);

        }
        break;
    }

    case Successful: {
        if (!processSuccessful()) {
            QuasarAppUtils::Params::log("Failed to parse package (processCardData)", QuasarAppUtils::Error);
            endWork(WrongPackage);

        }
        break;
    }
    default: {
        QuasarAppUtils::Params::log("Undefined command received",
                                    QuasarAppUtils::Error);
        endWork(WrongPackage);

        break;
    }
    }
}

void IConnectorBackEnd::handleConnectionClosed(ITargetNode* id) {
    if (_lastStatus == Error::InProgress) {
        connectionLost(id->nodeId());
        return;
    }
}

bool IConnectorBackEnd::processCardStatus(const QByteArray &message) {
    if (message.size() != sizeof(CardStatus)) {
        QuasarAppUtils::Params::log("CardStatus id is invalid", QuasarAppUtils::Error);
        return false;
    }

    // very dangerous
    CardStatus cardStatus = *reinterpret_cast<CardStatus*>(const_cast<char*>((message.data())));

    if (cardStatus.cardId == 0) {
        QuasarAppUtils::Params::log("card id is invalid", QuasarAppUtils::Error);
        return false;
    }

    if (_mode != Client) {
        return false;
    }

    Card userrquest;
    userrquest.setId(cardStatus.cardId);

    auto dbCard = _db->getObject(userrquest);

    if (!dbCard) {
        DataRequest request;
        request.command = CardDataRequest;

        if (!_currentTarget->sendMessage(QByteArray::fromRawData(reinterpret_cast<char*>(&request), sizeof(request)))) {
            QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);

            return false;
        }

        if (!_lastReceivedCardStatus)
            _lastReceivedCardStatus = new CardStatus;

        *_lastReceivedCardStatus = cardStatus;
        return true;

    }

    return applayPurchases(dbCard, cardStatus.purchasesCount);
}

bool IConnectorBackEnd::processStatusRequest(const QByteArray &message) {
    if (message.size() != sizeof(CardStatusRequest)) {
        QuasarAppUtils::Params::log("package size is invalid", QuasarAppUtils::Error);
        return false;
    }

    CardStatusRequest cardStatus = *reinterpret_cast<CardStatusRequest*>(const_cast<char*>((message.data())));

    auto sessionId = cardStatus.sessionId;

    QString where = QString("id IN (SELECT usersCardsID FROM Sessions WHERE id = %0)").
            arg(sessionId);
    QH::PKG::DBObjectsRequest<UsersCards> request("UsersCards", where);

    auto result = _db->getObject(request);
    if (!result || result->data().isEmpty()) {
        QuasarAppUtils::Params::log(QString("The session %0 is missing").
                                    arg(sessionId),
                                    QuasarAppUtils::Error);
        return false;
    }

    if (!sendCardStatus(result->data())) {
        return false;
    }

    return true;
}

bool IConnectorBackEnd::applayPurchases(QSharedPointer<RC::Card> dbCard,
                                        unsigned int purchases) {
    auto userCardsData = getUserCardData(_activeUser->userId(), dbCard->cardId());
    if (!userCardsData) {
        userCardsData = QSharedPointer<UsersCards>::create();
        userCardsData->setCard(dbCard->cardId());
        userCardsData->setOwner(false);
        userCardsData->setUser(_activeUser->userId());
    }
    userCardsData->setPurchasesNumber(purchases);

    if (!_db->insertIfExistsUpdateObject(userCardsData)) {
        QuasarAppUtils::Params::log("Failed to update data", QuasarAppUtils::Error);
        return false;
    }

    DataRequest responce;
    responce.command = Successful;
    if (!_currentTarget->sendMessage(QByteArray::fromRawData(reinterpret_cast<char*>(&responce), sizeof(responce)))) {
        QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);

        return false;
    }

    emit sigPurchaseWasSuccessful(userCardsData);
    endWork(FinishedSuccessful);

    return true;

}

bool IConnectorBackEnd::sendRawDataPackage(Commands command, const QByteArray &data) {

    QByteArray array;

    int size = data.size();

    array.push_back(command);
    array.push_back(QByteArray::fromRawData(reinterpret_cast<char*>(&size), sizeof (size)));
    array.push_back(data);

    return _currentTarget->sendMessage(array);
}

bool IConnectorBackEnd::extractRawData(const QByteArray &data, RawData &result) {
    if (data.size() <= 5) {
        return false;
    }

    result.command = *(reinterpret_cast<unsigned int*>(data.mid(0, 1).data()));
    unsigned int dataSize = result.size = *(reinterpret_cast<unsigned int*>(data.mid(1, 4).data()));

    if (static_cast<unsigned int>(data.size()) != dataSize + 5) {
        QuasarAppUtils::Params::log("user id is invalid", QuasarAppUtils::Error);
        return false;
    }

    result.data = data.mid(4);

    return true;

}

bool IConnectorBackEnd::processUserRequest(const QByteArray &message) {
    if (message.size() != sizeof(UserHeader)) {
        QuasarAppUtils::Params::log("user id is invalid", QuasarAppUtils::Error);
        return false;
    }

    // very dangerous
    UserHeader user = *reinterpret_cast<UserHeader*>(const_cast<char*>((message.data())));

    if (user.userId == 0) {
        QuasarAppUtils::Params::log("user id is invalid", QuasarAppUtils::Error);
        return false;
    }

    if (_mode == Client) {
        return false;
    }

    User userrquest;
    userrquest.setId(user.userId);

    auto dbUser = _db->getObject(userrquest);

    if (!dbUser) {

        dbUser = QSharedPointer<User>::create();
        dbUser->setKey(QByteArray::fromRawData(reinterpret_cast<char*>(&user.token), sizeof (user.token)));
        dbUser->setId(user.userId);

        if (!_db->insertIfExistsUpdateObject(dbUser)) {
            QuasarAppUtils::Params::log("Failed to update data", QuasarAppUtils::Warning);

        }

    }

    auto userCardsData = getUserCardData(user.userId, _activeCard->cardId());
    if (!userCardsData) {
        userCardsData = QSharedPointer<UsersCards>::create();
        userCardsData->setOwner(false);
        userCardsData->setUser(user.userId);
        userCardsData->setPurchasesNumber(0);
        userCardsData->setCard(_activeCard->cardId());
    }


    auto sessionData = QSharedPointer<Session>::create();
    sessionData->setUsercardId(UsersCards::genId(userCardsData->getUser(), userCardsData->getCard()));
    sessionData->setSessionId(user.sessionId);

    if (!_db->insertIfExistsUpdateObject(sessionData)) {
        QuasarAppUtils::Params::log("Failed to update session data", QuasarAppUtils::Error);
        return false;
    }

    if (_mode == Saller && !incrementPurchases(userCardsData)) {
        QuasarAppUtils::Params::log("Failed to update data", QuasarAppUtils::Error);

        return false;
    }

    if (!sendCardStatus(QList{userCardsData})) {
        return false;
    }

    emit sigPurchaseWasSuccessful(userCardsData);

    return true;
}

bool IConnectorBackEnd::processCardRequest(const QByteArray &message) {
    if (message.size() != sizeof(DataRequest)) {
        QuasarAppUtils::Params::log("user id is invalid", QuasarAppUtils::Error);
        return false;
    }

    // very dangerous
    DataRequest user = *reinterpret_cast<DataRequest*>(const_cast<char*>((message.data())));

    if (user.command == Undefined) {
        QuasarAppUtils::Params::log("user id is invalid", QuasarAppUtils::Error);
        return false;
    }

    if (_mode != Saller) {
        return false;
    }

    auto rawData = _activeCard->toBytes();

    unsigned int size = rawData.size();
    QByteArray responce;
    responce.push_back(CardData);
    responce.push_back(QByteArray::fromRawData(reinterpret_cast<char*>(&size), sizeof (size)));
    responce.push_back(rawData);

    if (!_currentTarget->sendMessage(responce)) {

        QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);

        return false;
    }

    return true;
}

bool IConnectorBackEnd::processCardData(const QByteArray &message) {

    if (message.size() <= 5) {
        return false;
    }

    unsigned int dataSize = *(reinterpret_cast<unsigned int*>(message.mid(1, 4).data()));

    if (static_cast<unsigned int>(message.size()) != dataSize + 5) {
        QuasarAppUtils::Params::log("user id is invalid", QuasarAppUtils::Error);
        return false;
    }

    if (_mode != Client) {
        return false;
    }

    auto card = QSharedPointer<Card>::create();
    card->fromBytes(message.right(dataSize));

    if (!card->isValid())
        return false;

    if (!_db->insertIfExistsUpdateObject(card)) {
        return false;
    }

    if (!_lastReceivedCardStatus) {
        return false;
    }

    auto userCardsData = getUserCardData(_activeUser->userId(), _lastReceivedCardStatus->cardId);
    if (!userCardsData) {
        userCardsData = QSharedPointer<UsersCards>::create();
        userCardsData->setUser(_activeUser->userId());
        userCardsData->setCard(_lastReceivedCardStatus->cardId);
        userCardsData->setOwner(false);
        userCardsData->setPurchasesNumber(_lastReceivedCardStatus->purchasesCount);
    }

    if (!_db->insertIfExistsUpdateObject(userCardsData)) {
        return false;
    }

    emit sigCardReceived(card);

    return applayPurchases(card, _lastReceivedCardStatus->purchasesCount);
}

bool IConnectorBackEnd::processSuccessful() {

    endWork(FinishedSuccessful);

    return true;
}

bool IConnectorBackEnd::sendStatusRequest(const QSharedPointer<Session> &sessionData) {
    CardStatusRequest status;
    status.command = StatusRequest;
    status.sessionId = sessionData->getSessionId();

    if (!_currentTarget->sendMessage(QByteArray::fromRawData(reinterpret_cast<char*>(&status), sizeof(status)))) {
        QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);
        return false;
    }

    return true;
}

bool IConnectorBackEnd::incrementPurchases(const QSharedPointer<UsersCards> &usersCardsData) {

    if (!usersCardsData)
        return false;

    unsigned int unixTime = time(0);

    unsigned long long uniqueCarduserId = usersCardsData->getUser();
    uniqueCarduserId = uniqueCarduserId | static_cast<unsigned long long>(usersCardsData->getCard()) << 32;

    if (unixTime - _lastUpdates.value(uniqueCarduserId) < TIME_LIMIT_SEC) {
        return false;
    }

    _lastUpdates[uniqueCarduserId] = unixTime;
    usersCardsData->setPurchasesNumber(usersCardsData->getPurchasesNumber() + _purchasesCount);


    return true;
}

void IConnectorBackEnd::beginWork() {
    _lastStatus = InProgress;
    emit sigSessionWasBegin();
    int workId;

    srand(rand());

    _workID = workId = rand();
    QTimer::singleShot(RC_WAIT_TIME, this, [this, workId]() {

        if (_workID != workId) {
            return ;
        }

        if (_lastStatus == InProgress) {
            endWork(TimeOut);
        }

    });
}

void IConnectorBackEnd::endWork(Error status) {
    _lastStatus = status;

    if (status != Error::FinishedSuccessful && status != Error::InProgress) {
        auto service = QmlNotificationService::NotificationService::getService();
        service->setNotify("NFC Error", "Eschanged is failed");
    }

    emit sigSessionWasFinshed(status);
    reset();

}

IConnectorBackEnd::Error IConnectorBackEnd::lastStatus() const {
    return _lastStatus;
}

IConnectorBackEnd::Mode IConnectorBackEnd::mode() const {
    return _mode;
}

void IConnectorBackEnd::setMode(Mode newMode) {
    _mode = newMode;
}

QSharedPointer<User> IConnectorBackEnd::activeUser() const {
    return _activeUser;
}

void IConnectorBackEnd::setActiveUser(QSharedPointer<User> newActiveUser) {
    _activeUser = newActiveUser;
}

QSharedPointer<Card> IConnectorBackEnd::activeCard() const {
    return _activeCard;
}

void IConnectorBackEnd::setActiveCard(QSharedPointer<Card> newActiveCard,
                                      int purchasesCount) {
    _activeCard = newActiveCard;
    _purchasesCount = purchasesCount;
}

}

