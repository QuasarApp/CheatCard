//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "apiv2.h"
#include "api0/carddatarequest.h"
#include "api0/cardstatusrequest.h"
#include "api0/contacts.h"
#include "api2/cardupdated.h"
#include "api2/changeuserscards.h"
#include "api2/deletecardrequest.h"
#include "api2/restoreresponce.h"
#include "api2/statusafterchanges.h"
#include "api2/updatecontactdata.h"
#include "api2/updatecontactdataresponce.h"

#include <cmath>
#include <dbobjectsrequest.h>
#include <rci/rcutils.h>


namespace RC {
namespace API {

ApiV2::ApiV2(const QSharedPointer<Interfaces::iDB>& dataBase, QH::AbstractNode* mainNode):
    ApiV1(dataBase, mainNode) {

}

void ApiV2::initSupportedCommands() {
    // for all nodes types.
    registerPackageType<QH::PKG::DataPack<API::V0::Card>>();
    registerPackageType<QH::PKG::DataPack<API::V1::UsersCards>>();
    registerPackageType<QH::PKG::DataPack<API::V1::Card>>();
    registerPackageType<QH::PKG::DataPack<API::V2::UsersCards>>();

    switch (static_cast<NodeType>(node()->nodeType())) {
    case NodeType::Visitor: {
        registerPackageType<QH::PKG::DataPack<API::V0::Contacts>>();
        registerPackageType<API::V2::UpdateContactDataResponce>();
        registerPackageType<API::V2::RestoreResponce>();

        break;
    }

    case NodeType::Seller: {
        registerPackageType<API::V0::CardStatusRequest>();
        registerPackageType<API::V0::CardDataRequest>();
        registerPackageType<QH::PKG::DataPack<API::V0::Contacts>>();
        registerPackageType<API::V2::ChangeUsersCards>();
        registerPackageType<API::V2::StatusAfterChanges>();
        registerPackageType<API::V2::CardUpdated>();
        registerPackageType<API::V2::UpdateContactDataResponce>();
        registerPackageType<API::V2::RestoreResponce>();
        break;
    }

    case NodeType::Server: {
        registerPackageType<API::V0::Session>();
        registerPackageType<API::V0::CardStatusRequest>();
        registerPackageType<QH::PKG::DataPack<API::V0::UsersCards>>();

        registerPackageType<API::V0::CardDataRequest>();
        registerPackageType<QH::PKG::DataPack<API::V0::Card>>();
        registerPackageType<API::V1::RestoreDataRequest>();

        registerPackageType<API::V2::ChangeUsersCards>();
        registerPackageType<API::V2::StatusAfterChanges>();
        registerPackageType<API::V2::CardUpdated>();
        registerPackageType<API::V2::RestoreResponce>();
        registerPackageType<API::V2::UpdateContactData>();
        registerPackageType<API::V2::DeleteCardRequest>();
        break;
    }

    default:
        break;
    }
}

int ApiV2::version() const {
    return 2;
}

QH::ParserResult ApiV2::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                     const QH::Header &pkgHeader,
                                     QH::AbstractNodeInfo *sender) {

    auto result = commandHandler<API::V0::Session>(this, &ApiV2::processSession,
                                                   pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V0::CardStatusRequest>(this, &ApiV2::processCardStatusRequest,
                                                        pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<API::V2::UsersCards>>(this,
                                                                    &ApiV2::processCardStatus,
                                                                    pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V0::CardDataRequest>(this, &ApiV2::processCardRequest,
                                                      pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<API::V1::Card>>(this, &ApiV2::processCardData,
                                                              pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V1::RestoreDataRequest>(this,
                                                         &ApiV2::processRestoreDataRequest,
                                                         pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V2::ChangeUsersCards>(this,
                                                       &ApiV2::processChanges,
                                                       pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V2::StatusAfterChanges>(this,
                                                         &ApiV2::processStatusAfterChanged,
                                                         pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V2::CardUpdated>(this,
                                                  &ApiV2::processCardUpdate,
                                                  pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }


    result = commandHandler<API::V2::UpdateContactData>(this,
                                                        &ApiV2::processContacts,
                                                        pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }


    result = commandHandler<API::V2::UpdateContactDataResponce>(this,
                                                                &ApiV2::processContactsResponce,
                                                                pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V2::RestoreResponce>(this,
                                                      &ApiV2::processRestoreResponce,
                                                      pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V2::DeleteCardRequest>(this,
                                                        &ApiV2::processDeleteCardRequest,
                                                        pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    return QH::ParserResult::NotProcessed;
}

void ApiV2::sendCardStatusRequest(long long userSession, QH::AbstractNodeInfo *dist) {
    V0::CardStatusRequest request;
    request.setSessionId(userSession);

    _checkUserRequestHash.clear();
    if (auto pkghash = node()->sendData(&request, dist)) {
        _checkUserRequestHash += pkghash;
    }
}

bool ApiV2::processCardStatus(const QSharedPointer<QH::PKG::DataPack<API::V2::UsersCards> > &cardStatuses,
                              const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {
    return ApiV2::processCardStatusImpl(*cardStatuses, sender, pkg);
}

bool ApiV2::processCardStatusImpl(const QH::PKG::DataPack<API::V2::UsersCards> &cardStatuses,
                                  const QH::AbstractNodeInfo *sender,
                                  const QH::Header &pkg) {
    V0::CardDataRequest request;

    for (const auto& cardStatus : cardStatuses.packData()) {
        unsigned int neededCardId = 0;
        if (!processCardStatusBase(cardStatus, cardStatuses.customData(),
                                   sender, pkg, neededCardId)) {
            break;
        }

        if (neededCardId) {
            request.push(neededCardId);
        }
    }

    if (request.getCardIds().size()) {

        if (!node()->sendData(&request, sender, &pkg)) {
            QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);

            return false;
        }

        return true;
    }

    return node()->removeNode(sender->networkAddress());
}

bool ApiV2::processDeleteCardRequest(const QSharedPointer<API::V2::DeleteCardRequest> &request,
                                     const QH::AbstractNodeInfo * sender,
                                     const QH::Header &) {

    auto dbCard = db()->getCard(request->card());
    if (!dbCard)
        return true;

    if (!accessValidation(dbCard, request->secret(), false)) {

        QuasarAppUtils::Params::log("Receive not signed delete request",
                                    QuasarAppUtils::Info);
        return false;
    }

    auto userKey = RCUtils::makeUserKey(request->secret());
    auto userId = RCUtils::makeUserId(userKey);
    auto listOfUsers = db()->getAllActiveUserFromCard(request->card(),
                                                      Interfaces::ACTIVE_USER_TIME_LIMIT, userId);

    if (listOfUsers.size()) {
        return false;
    }

    if (!db()->deleteCard(request->card())) {
        return false;
    }

    if (sender) {
        return node()->removeNode(sender->networkAddress());
    }

    return true;
}

bool ApiV2::processCardStatusBase(const QSharedPointer<V2::UsersCards> &cardStatus,
                                  const QByteArray& userSecreet,
                                  const QH::AbstractNodeInfo *sender,
                                  const QH::Header &pkg,
                                  unsigned int& neededCardId) {

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

    bool hasUpdate = !dbCard || dbCard->getCardVersion() < cardStatus->getCardVersion();

    if (hasUpdate) {
        neededCardId = cardStatus->getCard();
    }

    return true;
}

void ApiV2::processCardStatusWithoutCardRequests(
    const QSharedPointer<QH::PKG::DataPack<API::V2::UsersCards> > &cardStatuses) {

    for (const auto& cardStatus : cardStatuses->packData()) {
        auto dbCard = db()->getCard(cardStatus->getCard());
        auto dbUsersCards = db()->getUserCardData(
            cardStatus->getUser(),
            cardStatus->getCard());

        // ignore seels statuses that has a depricated time.
        if (dbUsersCards && dbUsersCards->getRawTime() > cardStatus->getRawTime()) {
            continue;
        }

        if (!accessValidation(dbCard, cardStatuses->customData(), true)) {

            QuasarAppUtils::Params::log("Receive not signed cards seal");
            break;
        }

        // Disable alert if this packge is ansver to restore request
        if (!applayPurchases(cardStatus, nullptr, false)) {
            break;
        }
    }

    return;
}

bool ApiV2::processCardRequest(const QSharedPointer<API::V0::CardDataRequest> &cardrequest,
                               const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {

    return ApiV1::processCardRequest(cardrequest, sender, pkg);

}

bool ApiV2::processCardData(const QSharedPointer<QH::PKG::DataPack<API::V1::Card>> &cards,
                            const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {

    return ApiV1::processCardData(cards, sender, pkg);

}

bool ApiV2::processCardUpdate(const QSharedPointer<API::V2::CardUpdated> &cardrequest,
                              const QH::AbstractNodeInfo *sender,
                              const QH::Header &hdr) {

    auto dbCard = db()->getCard(cardrequest->cardId());
    V0::CardDataRequest request;

    if (!dbCard || dbCard->getCardVersion() < cardrequest->cardVersion()) {
        request.push(cardrequest->cardId());
    }

    if (request.getCardIds().size()) {
        return node()->sendData(&request, sender, &hdr);
    }

    return node()->removeNode(sender->networkAddress());
}

void ApiV2::collectDataOfuser(const QByteArray& userKey, QH::PKG::DataPack<API::V2::UsersCards>& responce) {
    unsigned int userID = RCUtils::makeUserId(userKey);
    auto masterUser = db()->getMasterKeys(userKey);

    auto result = db()->getAllUserData(userID);

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

bool ApiV2::processRestoreDataRequest(const QSharedPointer<API::V1::RestoreDataRequest> &cardrequest,
                                      const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    API::V2::RestoreResponce responce;
    responce.setUserKey(cardrequest->userKey());
    QH::PKG::DataPack<API::V2::UsersCards> usersData;

    collectDataOfuser(cardrequest->userKey(), usersData);
    responce.setUsersCards(usersData);

    auto contacts = db()->getSlaveKeys(cardrequest->userKey());
    contacts += db()->getMasterKeys(cardrequest->userKey());

    QH::PKG::DataPack<API::V0::Contacts> contactsData;
    for (const auto& contact : qAsConst(contacts)) {
        contactsData.push(contact);
    }

    responce.setContacts(contactsData);

    if (responce.isValid()) {
        return node()->sendData(&responce, sender, &hdr);
    }

    return node()->removeNode(sender->networkAddress());
}

bool ApiV2::processStatusAfterChanged(const QSharedPointer<API::V2::StatusAfterChanges> &status,
                                      const QH::AbstractNodeInfo * sender,
                                      const QH::Header & hdr) {

    auto cardStatus = QSharedPointer<QH::PKG::DataPack<API::V2::UsersCards>>::create(status->getLastStatus());

    if (status->neededCard()) {
        processCardStatusWithoutCardRequests(cardStatus);

        auto card = db()->getCard(status->getCard());
        QH::PKG::DataPack<API::V1::Card> responce;
        responce.push(card);

        return node()->sendData(&responce, sender, &hdr);
    }

    if (!processCardStatus(cardStatus, sender, hdr)) {
        return false;
    }

    emit sigSessionStatusResult(status->toObject(db()), status->status());
    return true;
}

void ApiV2::applayUpdateContactData(const QSharedPointer<RC::API::V2::UpdateContactData> &data,
                                    bool success) {

    if (success) {
        if (data->getRemove()) {
            db()->deleteContact(data->toObject(db()));
        } else {
            db()->saveContact(data->toObject(db()));
        }
    }

    emit sigContactsStatusResult(data->toObject(db()),
                                 success, data->getRemove());
}

void ApiV2::processContactsResponcePrivate(unsigned int requestId, bool result) {

    if (auto object = _waitResponce.value(requestId)) {
        applayUpdateContactData(object, result);
        _waitResponce.remove(requestId);
    }
}

QH::PKG::DataPack<API::V2::UsersCards>
ApiV2::lastUserStatus(unsigned int cardId) {
    QH::PKG::DataPack<API::V2::UsersCards> responce;

    auto result = db()->getAllUserFromCard(cardId);

    for (const auto &data : qAsConst(result)) {
        data->setCardVersion(db()->getCardVersion(cardId));
        responce.push(data);
    }

    return responce;
}

bool ApiV2::processCardStatusRequest(const QSharedPointer<API::V0::CardStatusRequest> &cardStatus,
                                     const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {

    auto sessionId = cardStatus->getSessionId();

    auto result = db()->getUsersCardsFromSession(sessionId);
    if (result.isEmpty()) {
        QuasarAppUtils::Params::log(QString("The session %0 is missing").
                                    arg(sessionId),
                                    QuasarAppUtils::Debug);
        return node()->removeNode(sender->networkAddress());
    }

    QH::PKG::DataPack<API::V2::UsersCards> responce;

    QByteArray publicKey;
    for (const auto &data : qAsConst(result)) {
        data->setCardVersion(db()->getCardVersion(data->getCard()));
        responce.push(data);
        publicKey = db()->getPublic(data->getUser());
    }

    responce.setCustomData(db()->getSecret(publicKey));

    if (!node()->sendData(&responce, sender, &pkg)) {
        return false;
    }

    sessionProcessed(sessionId);

    return true;
}

bool ApiV2::processChanges(const QSharedPointer<API::V2::ChangeUsersCards> &message,
                           const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    if (!message->isValid()) {
        return false;
    }

    auto dbUsersCards = db()->getUserCardData(
        message->getUser(),
        message->getCard());

    API::V2::StatusAfterChanges status;
    status.setSessionId(message->getSessionId());
    status.setUsercardId(message->getUsercardId());

    if (dbUsersCards) {
        int availabelFreeItems = db()->getFreeItemsCount(dbUsersCards);
        if (availabelFreeItems < message->receive()) {
            // return status false and new users statuses of this seller

            auto lastStatus = db()->
                              getUserCardData(message->getUser(), message->getCard());

            status.setStatus(false);
            status.addLastStatus(QSharedPointer<API::V2::UsersCards>::create(lastStatus));
            if (!node()->sendData(&status, sender, &hdr)){
                return false;
            }
        }

    } else {
        dbUsersCards = db()->makeEmptyUsersCard();
        dbUsersCards->setUser(message->getUser());
        dbUsersCards->setCard(message->getCard());
    }

    db()->saveSession(message->toObject(db()));

    dbUsersCards->setPurchasesNumber(dbUsersCards->getPurchasesNumber() + message->purchase());
    dbUsersCards->setTime(time(0));
    dbUsersCards->receive(message->receive());

    unsigned int neededCardId = 0;
    if (!processCardStatusBase(QSharedPointer<V2::UsersCards>::create(dbUsersCards),
                               message->secret(), sender, hdr, neededCardId)) {
        return false;
    }

    status.setNeededCard(neededCardId);

    auto lastStatus = db()->
                      getUserCardData(message->getUser(), message->getCard());
    status.addLastStatus(QSharedPointer<API::V2::UsersCards>::create(lastStatus));
    status.setStatus(true);

    return node()->sendData(&status, sender, &hdr);
}

bool ApiV2::processSession(const QSharedPointer<API::V0::Session> &session,
                           const QH::AbstractNodeInfo *sender,
                           const QH::Header &pkg ) {

    return ApiV1::processSession(session, sender, pkg);
}

bool ApiV2::processRestoreResponce(const QSharedPointer<API::V2::RestoreResponce> &message,
                                   const QH::AbstractNodeInfo *sender,
                                   const QH::Header &hdr) {

    if (!db()->deleteContactsByChildUserKey(message->userKey())) {
        return false;
    }

    for (const auto& contact: message->contacts().packData()) {
        db()->saveContact(contact->toObject(db()));
    }

    if (!ApiV2::processCardStatusImpl(message->usersCards(), sender, hdr))
        return false;

    emit sigContactsListChanged();

    return true;
}

bool ApiV2::accessValidation(const QSharedPointer<RC::Interfaces::iCard> &cardFromDB,
                             const QByteArray &ownerSecret,
                             bool allowWorkers) const {
    switch (static_cast<NodeType>(node()->nodeType())) {
    case NodeType::Server: {
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

    default: {};
    }

    return true;
}

bool ApiV2::processContacts(const QSharedPointer<API::V2::UpdateContactData> &message,
                            const QH::AbstractNodeInfo * sender,
                            const QH::Header &hdr) {

    auto userKey = RCUtils::makeUserKey(message->userSecreet());
    auto userId = RCUtils::makeUserId(userKey);

    if (userId != message->getUser()) {
        QuasarAppUtils::Params::log("User try change permission for another user.",
                                    QuasarAppUtils::Info);
        return false;
    }

    if (message->getRemove()) {
        if (!db()->deleteContact(message->toObject(db()))) {
            QuasarAppUtils::Params::log("Fail to detele user permisiion",
                                        QuasarAppUtils::Error);
            return false;
        }

    } else {
        if (!db()->saveContact(message->toObject(db()))) {
            QuasarAppUtils::Params::log("Fail to save user permisiion",
                                        QuasarAppUtils::Error);
            return false;
        }
    }

    API::V2::UpdateContactDataResponce responce;
    responce.setSuccessful(true);

    return node()->sendData(&responce, sender, &hdr);
}

bool ApiV2::processContactsResponce(
    const QSharedPointer<API::V2::UpdateContactDataResponce> &message,
    const QH::AbstractNodeInfo *sender,
    const QH::Header & hdr) {

    processContactsResponcePrivate(hdr.triggerHash, message->getSuccessful());
    return node()->removeNode(sender->networkAddress());
}

void ApiV2::restoreOldDateRequest(const QByteArray &curentUserKey,
                                  QH::AbstractNodeInfo *dist) {
    ApiV1::restoreOldDateRequest(curentUserKey, dist);
}

void ApiV2::restoreOneCardRequest(unsigned int cardId, QH::AbstractNodeInfo *dist) {
    ApiV1::restoreOneCardRequest(cardId, dist);

}

void ApiV2::sendSessions(const QHash<long long, QSharedPointer<Interfaces::iSession> > &sessions,
                         QH::AbstractNodeInfo *dist) {

    _checkUserRequestHash.clear();
    for (const auto &session: qAsConst(sessions)) {
        V0::Session srssionObj(session);
        if (unsigned int pkgHash = node()->sendData(&srssionObj, dist)) {
            _checkUserRequestHash += pkgHash;
        }
    }
}

bool ApiV2::sendContacts(const Interfaces::iContacts& contact,
                         const QByteArray& secreet,
                         bool removeRequest,
                         QH::AbstractNodeInfo *dist) {


    auto request = QSharedPointer<RC::API::V2::UpdateContactData>::create(contact);
    request->setRemove(removeRequest);
    request->setUserSecreet(secreet);

    unsigned int pkgHash = node()->sendData(request.data(), dist);

    if (!pkgHash) {
        return false;
    }

    _waitResponce[pkgHash] = request;

    QTimer::singleShot(10000, nullptr, [this, pkgHash]() {
        processContactsResponcePrivate(pkgHash, false);
    });

    return true;
}

bool ApiV2::deleteCard(unsigned int cardId,
                       const QByteArray &curentUserKey,
                       QH::AbstractNodeInfo *dist) {
    API::V2::DeleteCardRequest request;
    request.setCard(cardId);
    auto secret = db()->getSecret(curentUserKey);

    if (secret.isEmpty())
        return false;

    request.setSecret(secret);

    return node()->sendData(&request, dist, nullptr);
}

bool ApiV2::sendUpdateCard(unsigned int cardId,
                           unsigned int version,
                           QH::AbstractNodeInfo *dist) {
    API::V2::CardUpdated request;
    request.setCardVersion(version);
    request.setCardId(cardId);

    return node()->sendData(&request, dist, nullptr);

}

bool ApiV2::changeUsersData(const QByteArray& sellerUserKey,
                            unsigned int cardId,
                            unsigned int userId,
                            unsigned long long session,
                            unsigned int purchasesCount,
                            unsigned int receivedCount,
                            QH::AbstractNodeInfo *dist) {

    API::V2::ChangeUsersCards changes;

    changes.setUsercardId(RCUtils::makeUsersCardsId(userId, cardId));
    changes.setSessionId(session);
    changes.setPurchase(purchasesCount);
    changes.setReceive(receivedCount);
    changes.setSecret(db()->getSecret(sellerUserKey));

    return sendData(&changes, dist);

}

}

}
