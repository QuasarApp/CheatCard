//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "apiv3.h"
#include "api3/carddatarequest.h"
#include "api3/contacts.h"
#include "api3/cardupdated.h"
#include "api3/changeuserscards.h"
#include "api3/deletecardrequest.h"
#include "api3/syncincremental.h"
#include "api3/updatecontactdata.h"
#include "api3/userscards.h"

#include <api3/card.h>
#include <api3/subscribetouserchanges.h>
#include <api3/sync.h>
#include <api3/versionforoldapp.h>
#include <cmath>
#include <dbobjectsrequest.h>
#include <rci/rcutils.h>


namespace RC {
namespace API {

ApiV3::ApiV3(const QSharedPointer<Interfaces::iDB>& dataBase, QH::AbstractNode* mainNode):
    APIBase(dataBase, mainNode) {

}

void ApiV3::initSupportedCommands() {
    // for all nodes types.
    registerPackageType<QH::PKG::DataPack<API::V3::Card>>();
    registerPackageType<QH::PKG::DataPack<API::V3::UsersCards>>();
    registerPackageType<API::V3::CardUpdated>();
    registerPackageType<API::V3::CardDataRequest>();

    switch (node()->nodeType()) {
    case QH::AbstractNode::NodeType::Client: {
        registerPackageType<API::V3::Sync>();
        registerPackageType<API::V3::SyncIncremental>();

        break;
    }

    case QH::AbstractNode::NodeType::Server: {

        registerPackageType<API::V3::ChangeUsersCards>();
        registerPackageType<API::V3::UpdateContactData>();
        registerPackageType<API::V3::DeleteCardRequest>();
        registerPackageType<API::V3::SubscribeToUserChanges>();
        break;
    }

    default:
        break;
    }
}

int ApiV3::version() const {
    return 3;
}

QH::ParserResult ApiV3::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                     const QH::Header &pkgHeader,
                                     QH::AbstractNodeInfo *sender) {


    auto result = commandHandler<QH::PKG::DataPack<API::V3::UsersCards>>(this,
                                                                         &ApiV3::processCardStatus,
                                                                         pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V3::CardDataRequest>(this, &ApiV3::processCardRequest,
                                                      pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<API::V3::Card>>(this, &ApiV3::processCardData,
                                                              pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V3::ChangeUsersCards>(this,
                                                       &ApiV3::processChanges,
                                                       pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V3::Sync>(this,
                                           &ApiV3::processSync,
                                           pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V3::SyncIncremental>(this,
                                           &ApiV3::processSyncIncremental,
                                           pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V3::CardUpdated>(this,
                                                  &ApiV3::processCardUpdate,
                                                  pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }


    result = commandHandler<API::V3::UpdateContactData>(this,
                                                        &ApiV3::processContacts,
                                                        pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V3::DeleteCardRequest>(this,
                                                        &ApiV3::processDeleteCardRequest,
                                                        pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V3::SubscribeToUserChanges>(this,
                                                        &ApiV3::processSubscribeRequest,
                                                        pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }


    return QH::ParserResult::NotProcessed;
}

void ApiV3::syncRequest(const QByteArray &curentUserKey,
                        QH::AbstractNodeInfo *dist,
                        const std::function<void (bool)> &cb) {

    V3::SubscribeToUserChanges request;
    request.setUserId(curentUserKey);

    if (!sendAndRegisterCallBack(&request, dist, cb)) {
        QuasarAppUtils::Params::log("Failed to register user on server",
                                    QuasarAppUtils::Error);

    }

}

bool ApiV3::processCardStatus(const QSharedPointer<QH::PKG::DataPack<API::V3::UsersCards> > &cardStatuses,
                              const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {
    return ApiV3::processCardStatusImpl(*cardStatuses, sender, pkg);
}

bool ApiV3::processCardStatusImpl(const QH::PKG::DataPack<API::V3::UsersCards> &cardStatuses,
                                  const QH::AbstractNodeInfo *sender,
                                  const QH::Header &pkg) {
    V3::CardDataRequest request;

    for (const auto& cardStatus : cardStatuses.packData()) {
        QByteArray neededCardId;
        if (!processCardStatusBase(cardStatus, cardStatuses.customData(),
                                   sender, pkg, &neededCardId)) {
            break;
        }

        if (neededCardId.size()) {
            request.push(neededCardId);
        }
    }

    if (request.getCardIds().size()) {

        if (!node()->sendData(&request, sender, &pkg)) {
            QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);

            return false;
        }
    }

    return true;
}

bool ApiV3::processDeleteCardRequest(const QSharedPointer<API::V3::DeleteCardRequest> &request,
                                     const QH::AbstractNodeInfo *,
                                     const QH::Header & hdr) {

    auto dbCard = db()->getCard(request->card());
    if (!dbCard)
        return true;

    if (!accessValidation(dbCard, request->secret(), false)) {

        QuasarAppUtils::Params::log("Receive not signed delete request",
                                    QuasarAppUtils::Info);
        return false;
    }

    auto userKey = RCUtils::makeUserKey(request->secret());
    auto listOfUsers = db()->getAllActiveUserFromCard(request->card(),
                                                      Interfaces::ACTIVE_USER_TIME_LIMIT, userKey);

    if (listOfUsers.size()) {
        return false;
    }

    if (!db()->deleteCard(request->card())) {
        return false;
    }

    const auto usersDataToRemove = db()->getAllPassiveUserFromCard(request->card(), 0);
    QH::PKG::DataPack<API::V3::UsersCards> toRemove;
    for (const auto& data: usersDataToRemove) {
        toRemove.push({data});
    }

    API::V3::SyncIncremental changesResponce;
    changesResponce.setUsersCardsToRemove(toRemove);

    brodcastCardChanged(request->card(), dbCard->ownerSignature(), &changesResponce, &hdr);

    return true;
}

bool ApiV3::processCardStatusBase(const QSharedPointer<V3::UsersCards> &cardStatus,
                                  const QByteArray& userSecreet,
                                  const QH::AbstractNodeInfo *sender,
                                  const QH::Header &pkg,
                                  QByteArray* neededCardId) {

    auto dbCard = db()->getCard(cardStatus->getCard());
    auto dbUsersCards = db()->getUserCardData(
                cardStatus->getUser(),
                cardStatus->getCard());

    if (!accessValidation(dbCard, userSecreet, true)) {

        QuasarAppUtils::Params::log("Receive not signed cards seal",
                                    QuasarAppUtils::Info);
        return false;
    }

    // Disable alert if this packge is ansver to restore request
    if (!applayPurchases(cardStatus,
                         sender, _checkUserRequestHash.contains(pkg.triggerHash))) {
        return false;
    }

    if (neededCardId) {
        bool hasUpdate = !dbCard || dbCard->getCardVersion() < cardStatus->getCardVersion();

        if (hasUpdate) {
            *neededCardId = cardStatus->getCard();
        }
    }

    return true;
}

bool ApiV3::cardValidation(const QSharedPointer<Interfaces::iCard> &cardFromDB,
                           const QByteArray &ownerSecret) const {

    if (isServer()) {
        if (!(cardFromDB && cardFromDB->isValid()))
            return true;

        auto signature = cardFromDB->ownerSignature();

        if (signature.isEmpty()) {
            return true;
        }

        auto ownerSignature =  RCUtils::makeUserKey(ownerSecret);

        return signature == ownerSignature;
    }

    return true;
}

bool ApiV3::processCardRequest(const QSharedPointer<API::V3::CardDataRequest> &cardrequest,
                               const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {


    QH::PKG::DataPack<API::V3::Card> cards{};

    QByteArray publicKey;
    for (const auto& cardId : cardrequest->getCardIds()) {
        auto card = db()->getCard(cardId);

        if (!card) {
            if (isClient()) {
                QuasarAppUtils::Params::log(QString("Failed to find card with id: %0").
                                            arg(QString(cardId.toBase64())),
                                            QuasarAppUtils::Warning);
            }

            continue;
        }
        cards.push(card);
        publicKey = card->ownerSignature();
    }

    if (!cards.packData().size()) {
        if (isClient()) {
            QuasarAppUtils::Params::log(QString("Failed to find any cards "),
                                        QuasarAppUtils::Error);
            return false;
        }
        return true;
    }

    cards.setCustomData(db()->getSecret(publicKey));

    if (!node()->sendData(&cards, sender, &pkg)) {

        QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);
        return false;
    }

    return true;
}

bool ApiV3::processCardData(const QSharedPointer<QH::PKG::DataPack<API::V3::Card>> &cards,
                            const QH::AbstractNodeInfo *, const QH::Header &hdr) {

    if (!(cards && db())) {
        return false;
    }


    for (const auto &card: qAsConst(cards->packData())) {

        if (!card->isValid()) {
            QuasarAppUtils::Params::log("Received invalid card data!",
                                        QuasarAppUtils::Error);
            continue;
        }
        auto dbCard = db()->getCard(card->id());
        if (!cardValidation(dbCard, cards->customData())) {

            QuasarAppUtils::Params::log("Receive not signed card",
                                        QuasarAppUtils::Error);
            return false;
        }

        if (dbCard && card->ownerSignature() != dbCard->ownerSignature()) {
            QuasarAppUtils::Params::log("User try change the card owner signature!!!",
                                        QuasarAppUtils::Error);
            return false;
        }

        if (dbCard && dbCard->getCardVersion() >= card->getCardVersion()) {
            QuasarAppUtils::Params::log("Received outdatedCard",
                                        QuasarAppUtils::Warning);
            continue;
        }

        auto cardObj = card->toObject(db());
        if (!db()->saveCard(cardObj)) {
            continue;
        }

        if (isServer()) {
            API::V3::SyncIncremental changesResponce;
            changesResponce.addCardToUpdate(cardObj->cardId(), cardObj->getCardVersion());
            brodcastCardChanged(cardObj->cardId(), cardObj->ownerSignature(), &changesResponce, &hdr);
        }

        emit sigCardReceived(cardObj);
    }


    return true;
}

bool ApiV3::processCardUpdate(const QSharedPointer<API::V3::CardUpdated> &cardrequest,
                              const QH::AbstractNodeInfo *sender,
                              const QH::Header &hdr) {
    return processCardUpdatePrivate(cardrequest->cardId(), cardrequest->cardVersion(), sender, hdr);
}

bool ApiV3::applayPurchases(const QSharedPointer<API::V3::UsersCards> &dbCard,
                            const QH::AbstractNodeInfo *, bool alert) {

    auto usersCard = dbCard->toObject(db());
    if (!db()->saveUsersCard(usersCard)) {
        QuasarAppUtils::Params::log("Failed to update data", QuasarAppUtils::Error);
        return false;
    }

    emit sigPurchaseWasSuccessful(usersCard, alert);

    return true;

}

bool ApiV3::processChanges(const QSharedPointer<API::V3::ChangeUsersCards> &message,
                           const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    if (!message->isValid()) {
        return false;
    }

    auto dbUsersCards = db()->getUserCardData(
                message->getUser(),
                message->getCard());

    API::V3::SyncIncremental changesResponce;
    changesResponce.setResult(true);

    if (!dbUsersCards) {
        dbUsersCards = db()->makeEmptyUsersCard();
        dbUsersCards->setCard(message->getCard());
        dbUsersCards->setUser(message->getUser());
    }

    int availabelFreeItems = db()->getFreeItemsCount(dbUsersCards);
    if (availabelFreeItems < message->receive()) {
        // return status false and new users statuses of this seller
        changesResponce.setResult(false);
    } else {

        dbUsersCards->setPurchasesNumber(dbUsersCards->getPurchasesNumber() + message->purchase());
        dbUsersCards->setTime(time(0));
        dbUsersCards->receive(message->receive());

        QByteArray neededCardId;
        if (!processCardStatusBase(QSharedPointer<V3::UsersCards>::create(dbUsersCards),
                                   message->secret(), sender, hdr, &neededCardId)) {
            return false;
        }

        if (neededCardId.size()) {
            V3::CardDataRequest request;
            request.setCardIds({neededCardId});
            if (!sendData(&request, sender, &hdr)) {
                QuasarAppUtils::Params::log("Failed to sent card request.",
                                            QuasarAppUtils::Error);
                return false;
            }

        }
    }

    // responce

    auto lastStatus = db()->
            getUserCardData(message->getUser(), message->getCard());
    changesResponce.addUsersCardsToAdd(QSharedPointer<API::V3::UsersCards>::create(lastStatus));

    // message for all subscribers.
    brodcastUserChanged(message->getUser(), &changesResponce, &hdr);

    auto owner = db()->getCardField(message->getCard(), "ownerSignature").toByteArray();
    if (!owner.isEmpty()) {
        const auto workers = db()->getSlaveKeys(owner);
        for (const auto& worker: workers) {
            brodcastUserChanged(worker->getChildUserKey(), &changesResponce, &hdr);
        }
    }

    // responce for the sender.
    return sendData(&changesResponce, sender, &hdr);
}

bool ApiV3::accessValidation(const QSharedPointer<RC::Interfaces::iCard> &cardFromDB,
                             const QByteArray &ownerSecret,
                             bool allowWorkers) const {

    if (isServer()) {
        if (!(cardFromDB && cardFromDB->isValid()))
            return true;

        auto signature = cardFromDB->ownerSignature();
        auto ownerSignature =  RCUtils::makeUserKey(ownerSecret);

        if (signature == ownerSignature)
            return true;

        if (!allowWorkers)
            return false;

        // check additional access
        auto contact = db()->getContactFromChildId(signature, ownerSignature);
        return contact;
    }

    return true;
}

void ApiV3::brodcastUserChanged(const QByteArray &userId,
                     const QH::PKG::AbstractData *data,
                     const QH::Header *req) {

    QMutexLocker lock(&_subscribesMutex);

    const auto subscribers = _subscribes.value(userId);
    for (const auto& subscriber: subscribers) {
        if (subscriber->isConnected())
            sendData(data, subscriber, req);
        else {
            _subscribes[userId].remove(subscriber);
        }
    }
}

void ApiV3::brodcastCardChanged(const QByteArray &cardId,
                                const QByteArray &ownerId,
                                const QH::PKG::AbstractData *data,
                                const QH::Header *req) {
    if (!db())
        return;

    auto usersData = db()->getAllUserDataFromCard(cardId);
    auto workersUsers = db()->getSlaveKeys(ownerId);

    for (const auto& user: usersData) {
        brodcastUserChanged(user->getUser(), data, req);
    }

    for (const auto& user: workersUsers) {
        brodcastUserChanged(user->getChildUserKey(), data, req);
    }

    if (ownerId.size())
        brodcastUserChanged(ownerId, data, req);
}

bool ApiV3::processCardUpdatePrivate(const QByteArray &card,
                                     unsigned int version,
                                     const QH::AbstractNodeInfo *sender,
                                     const QH::Header &hdr) {
    auto dbCard = db()->getCard(card);
    V3::CardDataRequest request;

    // card needed for server if is a new card and if version updated. For client only for new version and onle for exists cards.
    bool fNeeded = node()->nodeType() == QH::AbstractNode::NodeType::Server && !dbCard;
    fNeeded = fNeeded || (dbCard && dbCard->getCardVersion() < version);

    if (fNeeded) {
        request.push(card);
    }

    if (request.getCardIds().size()) {
        return node()->sendData(&request, sender, &hdr);
    }

    return true;
}

bool ApiV3::processContacts(const QSharedPointer<API::V3::UpdateContactData> &message,
                            const QH::AbstractNodeInfo *,
                            const QH::Header &hdr) {

    auto userKey = RCUtils::makeUserKey(message->userSecreet());

    QSet<QByteArray> _brodcastFilter;

    auto brodcastChanges = [&_brodcastFilter, &hdr, this](const QByteArray& _key, const API::V3::SyncIncremental& resp){
        if (!_brodcastFilter.contains(_key)) {
            brodcastUserChanged(_key, &resp, &hdr);
        }
        _brodcastFilter += _key;
    };

    for (const auto & contact: message->contacts().packData()) {
        if (userKey != contact->getUserKey()) {
            QuasarAppUtils::Params::log("User try change permission for another user.",
                                        QuasarAppUtils::Info);
            return false;
        }

        API::V3::SyncIncremental responce;

        if (message->getRemove()) {
            if (!db()->deleteContact(contact->toObject(db()))) {
                QuasarAppUtils::Params::log("Fail to detele user permisiion",
                                            QuasarAppUtils::Error);
                return false;
            }
            responce.addContactsToRemove(contact);

        } else {
            if (!db()->saveContact(contact->toObject(db()))) {
                QuasarAppUtils::Params::log("Fail to save user permisiion",
                                            QuasarAppUtils::Error);
                return false;
            }
            responce.addContactsToAdd(contact);
        }

        responce.setResult(true);

        brodcastChanges(contact->getChildUserKey(), responce);
        brodcastChanges(contact->getUserKey(), responce);
    }

    return true;
}

bool ApiV3::triggerCallBack(unsigned int hash, unsigned int err) {
    auto cbData = _waitResponce.take(hash);
    if (cbData._cb)
        cbData._cb(err);

    if (_waitResponce.size() > 100) {
        for (auto it = _waitResponce.begin(); it != _waitResponce.end(); ++it) {
            if (it->time > time(0) + 60) {
                _waitResponce.erase(it);
                it = std::prev(it);
            }
        }
    }

    return true;
}

unsigned int ApiV3::sendAndRegisterCallBack(const QH::PKG::AbstractData *resp,
                                    const QH::AbstractNodeInfo *address,
                                    const std::function<void(int err)>& cb) {

    unsigned int pkgHash = sendData(resp, address);

    if (!pkgHash) {
        return false;
    }

    _waitResponce[pkgHash] = {static_cast<int>(time(0)), cb};

    return pkgHash;
}

bool ApiV3::sendContacts(const QSharedPointer<Interfaces::iContacts>& contact,
                         const QByteArray& secreet,
                         bool removeRequest,
                         QH::AbstractNodeInfo *dist,
                         const std::function<void(int err)>& cb) {


    RC::API::V3::UpdateContactData request;
    request.addContact(QSharedPointer<API::V3::Contacts>::create(contact));
    request.setRemove(removeRequest);
    request.setUserSecreet(secreet);

    return sendAndRegisterCallBack(&request, dist, cb);

}

bool ApiV3::deleteCard(const QByteArray& cardId,
                       const QByteArray& curentUserKey,
                       QH::AbstractNodeInfo *dist,
                       const std::function<void(int err)>& cb) {
    API::V3::DeleteCardRequest request;
    request.setCard(cardId);
    auto secret = db()->getSecret(curentUserKey);

    if (secret.isEmpty())
        return false;

    request.setSecret(secret);
    return sendAndRegisterCallBack(&request, dist, cb);

}

bool ApiV3::requestCard(const QByteArray &cardId,
                        QH::AbstractNodeInfo *dist,
                        const std::function<void (int)> &cb) {
    V3::CardDataRequest request;
    request.setCardIds({cardId});

    return sendAndRegisterCallBack(&request, dist, cb);
}

bool ApiV3::sendUpdateCard(const QByteArray& cardId,
                           unsigned int version,
                           QH::AbstractNodeInfo *dist,
                           const std::function<void(int err)>& cb) {
    API::V3::CardUpdated request;
    request.setCardVersion(version);
    request.setCardId(cardId);

    return sendAndRegisterCallBack(&request, dist, cb);
}

bool ApiV3::changeUsersData(const QByteArray& sellerUserKey,
                            const QByteArray& cardId,
                            const QByteArray& userId,
                            unsigned int purchasesCount,
                            unsigned int receivedCount,
                            QH::AbstractNodeInfo *dist,
                            const std::function<void(int err)>& cb) {

    API::V3::ChangeUsersCards changes;

    changes.setPurchase(purchasesCount);
    changes.setReceive(receivedCount);
    changes.setSecret(db()->getSecret(sellerUserKey));
    changes.setUser(userId);
    changes.setCard(cardId);

    _checkUserRequestHash.clear();

    auto packageHash = sendAndRegisterCallBack(&changes, dist, cb);
    _checkUserRequestHash += packageHash;

    return packageHash;
}

void ApiV3::sendOldVersionPackage(QH::AbstractNodeInfo *dist) {
    V3::VersionForOldApp oldVersionData;
    sendData(&oldVersionData, dist);
}

void ApiV3::collectDataOfuser(const QByteArray& userKey,
                              QH::PKG::DataPack<API::V3::UsersCards>& responce) {
    auto masterUser = db()->getMasterKeys(userKey);

    auto result = db()->getAllUserData(userKey);

    for (const auto &data : qAsConst(result)) {
        data->setCardVersion(db()->getCardVersion(data->getCard()));
        responce.push(data);
    }

    const auto datalist = db()->getAllUserCardsData(userKey, masterUser);
    for (const auto& item: datalist) {
        item->setCardVersion(db()->getCardVersion(item->getCard()));
        responce.push(item);
    }

    return;
}

bool ApiV3::processSubscribeRequest(const QSharedPointer<V3::SubscribeToUserChanges> &message,
                                    QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    _subscribesMutex.lock();
    _subscribes[message->userId()].insert(sender);
    _subscribesMutex.unlock();

    if (auto database = db()) {
        if (!database->migrateUsersCardsToUsersData(message->userId())) {
            return false;
        }
    }

    API::V3::Sync responce;
    responce.setSyncedUserKey(message->userId());
    QH::PKG::DataPack<API::V3::UsersCards> usersData;

    collectDataOfuser(message->userId(), usersData);
    responce.setUsersCards(usersData);

    auto contacts = db()->getSlaveKeys(message->userId());
    contacts += db()->getMasterKeys(message->userId());

    QH::PKG::DataPack<API::V3::Contacts> contactsData;
    for (const auto& contact : qAsConst(contacts)) {
        contactsData.push(contact);
    }

    responce.setContacts(contactsData);
    responce.setMode(V3::Sync::All);

    if (responce.isValid()) {
        return node()->sendData(&responce, sender, &hdr);
    }

    return true;
}

bool ApiV3::processSync(const QSharedPointer<V3::Sync> &message,
                        const QH::AbstractNodeInfo *sender,
                        const QH::Header & hdr) {

    if (message->isContainsUsersDataInfo() &&
            !ApiV3::processCardStatusImpl(message->usersCards(), sender, hdr)) {
        return false;
    }

    if (message->isContainsPermisionsInfo()) {
        if (!db()->deleteContactsByChildUserKey(message->syncedUserKey())) {
            return false;
        }

        for (const auto& contact: message->contacts().packData()) {
            db()->saveContact(contact->toObject(db()));
        }

        emit sigContactsListChanged();
    }

    return triggerCallBack(hdr.triggerHash, 0);
}

bool ApiV3::processSyncIncremental(const QSharedPointer<V3::SyncIncremental> &message,
                                   const QH::AbstractNodeInfo *sender,
                                   const QH::Header &hdr) {

    if (message->usersCardsToAdd().size() &&
            !ApiV3::processCardStatusImpl(message->usersCardsToAdd(), sender, hdr)) {
        return false;
    }

    const auto updateCards = message->cardUpdated();
    for (auto it = updateCards.begin(); it !=  updateCards.end(); it++) {
        processCardUpdatePrivate(it.key(), it.value(), sender, hdr);
    }

    for (const auto& userData: message->usersCardsToRemove().packData()) {
        db()->deleteUserData(userData->getCard(), userData->getUser());

        if (isClient()) {
            db()->deleteCard(userData->getCard());
        }

        emit sigUserDataRemoved(userData->getCard(), userData->getUser());
    }

    for (const auto& permisionData: message->contactsToAdd().packData()) {
        db()->saveContact(permisionData->toObject(db()));
    }

    for (const auto& permisionData: message->contactsToRemove().packData()) {
        db()->deleteContact(permisionData->toObject(db()));
    }

    if (message->contactsToAdd().size() || message->contactsToRemove().size()) {
        emit sigContactsListChanged();
    }

    return triggerCallBack(hdr.triggerHash, 0);
}

}

}
