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
#include "api3/updatecontactdata.h"
#include "api3/userscards.h"

#include <api3/card.h>
#include <api3/sync.h>
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

    switch (static_cast<NodeType>(node()->nodeType())) {
    case NodeType::Visitor: {
        registerPackageType<QH::PKG::DataPack<API::V3::Contacts>>();
        registerPackageType<API::V3::Sync>();

        break;
    }

    case NodeType::Seller: {
        registerPackageType<API::V3::Sync>();
        registerPackageType<API::V3::CardDataRequest>();
        registerPackageType<QH::PKG::DataPack<API::V3::Contacts>>();
        break;
    }

    case NodeType::Server: {
        registerPackageType<QH::PKG::DataPack<API::V3::UsersCards>>();

        registerPackageType<API::V3::CardDataRequest>();
        registerPackageType<QH::PKG::DataPack<API::V3::Card>>();

        registerPackageType<API::V3::ChangeUsersCards>();
        registerPackageType<API::V3::CardUpdated>();
        registerPackageType<API::V3::UpdateContactData>();
        registerPackageType<API::V3::DeleteCardRequest>();
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

    return QH::ParserResult::NotProcessed;
}

//void ApiV3::sendCardStatusRequest(long long userSession, QH::AbstractNodeInfo *dist) {
//    V3::CardStatusRequest request;
//    request.setSessionId(userSession);

//    _checkUserRequestHash.clear();
//    if (auto pkghash = node()->sendData(&request, dist)) {
//        _checkUserRequestHash += pkghash;
//    }
//}

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
                                   sender, pkg, neededCardId)) {
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

        return true;
    }

    return node()->removeNode(sender->networkAddress());
}

bool ApiV3::processDeleteCardRequest(const QSharedPointer<API::V3::DeleteCardRequest> &request,
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
    auto listOfUsers = db()->getAllActiveUserFromCard(request->card(),
                                                      Interfaces::ACTIVE_USER_TIME_LIMIT, userKey);

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

bool ApiV3::processCardStatusBase(const QSharedPointer<V3::UsersCards> &cardStatus,
                                  const QByteArray& userSecreet,
                                  const QH::AbstractNodeInfo *sender,
                                  const QH::Header &pkg,
                                  QByteArray& neededCardId) {

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

bool ApiV3::cardValidation(const QSharedPointer<Interfaces::iCard> &cardFromDB,
                           const QByteArray &ownerSecret) const {

    switch (static_cast<NodeType>(node()->nodeType())) {
    case NodeType::Server: {
        if (!(cardFromDB && cardFromDB->isValid()))
            return true;

        auto signature = cardFromDB->ownerSignature();

        if (signature.isEmpty()) {
            return true;
        }

        auto ownerSignature =  RCUtils::makeUserKey(ownerSecret);

        return signature == ownerSignature;
    }

    default: {};
    }

    return true;
}

void ApiV3::processCardStatusWithoutCardRequests(
        const QSharedPointer<QH::PKG::DataPack<API::V3::UsersCards> > &cardStatuses) {

    for (const auto& cardStatus : cardStatuses->packData()) {
        auto dbCard = db()->getCard(cardStatus->getCard());
        auto dbUsersCards = db()->getUserCardData(
                    cardStatus->getUser(),
                    cardStatus->getCard());

        // ignore seels statuses that has a depricated time.
        if (dbUsersCards && dbUsersCards->getRawTime() > cardStatus->time()) {
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

bool ApiV3::processCardRequest(const QSharedPointer<API::V3::CardDataRequest> &cardrequest,
                               const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {


    QH::PKG::DataPack<API::V3::Card> cards{};

    QByteArray publicKey;
    for (const auto& cardId : cardrequest->getCardIds()) {
        auto card = db()->getCard(cardId);

        if (!card) {
            QuasarAppUtils::Params::log(QString("Failed to find card with id: %0").
                                        arg(QString(cardId.toBase64(QByteArray::Base64UrlEncoding))),
                                        QuasarAppUtils::Error);
            continue;
        }
        cards.push(card);
        publicKey = card->ownerSignature();
    }

    if (!cards.packData().size()) {
        QuasarAppUtils::Params::log(QString("Failed to find any cards "),
                                    QuasarAppUtils::Error);
        return false;
    }

    cards.setCustomData(db()->getSecret(publicKey));

    if (!node()->sendData(&cards, sender, &pkg)) {

        QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);
        return false;
    }

    return true;
}

bool ApiV3::processCardData(const QSharedPointer<QH::PKG::DataPack<API::V3::Card>> &cards,
                            const QH::AbstractNodeInfo *sender, const QH::Header &) {

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

        auto cardObj = card->toObject(db());
        if (!db()->saveCard(cardObj)) {
            continue;
        }

        emit sigCardReceived(cardObj);
    }

    return node()->removeNode(sender->networkAddress());
}

bool ApiV3::processCardUpdate(const QSharedPointer<API::V3::CardUpdated> &cardrequest,
                              const QH::AbstractNodeInfo *sender,
                              const QH::Header &hdr) {

    auto dbCard = db()->getCard(cardrequest->cardId());
    V3::CardDataRequest request;

    if (!dbCard || dbCard->getCardVersion() < cardrequest->cardVersion()) {
        request.push(cardrequest->cardId());
    }

    if (request.getCardIds().size()) {
        return node()->sendData(&request, sender, &hdr);
    }

    return node()->removeNode(sender->networkAddress());
}

void ApiV3::collectDataOfuser(const QByteArray& userKey, QH::PKG::DataPack<API::V3::UsersCards>& responce) {
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

QH::PKG::DataPack<API::V3::UsersCards>
ApiV3::lastUserStatus(const QByteArray& cardId) {
    QH::PKG::DataPack<API::V3::UsersCards> responce;

    auto result = db()->getAllUserFromCard(cardId);

    for (const auto &data : qAsConst(result)) {
        data->setCardVersion(db()->getCardVersion(cardId));
        responce.push(data);
    }

    return responce;
}

bool ApiV3::processChanges(const QSharedPointer<API::V3::ChangeUsersCards> &message,
                           const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    if (!message->isValid()) {
        return false;
    }

    auto dbUsersCards = db()->getUserCardData(
                message->getUser(),
                message->getCard());

  //  API::V3::StatusAfterChanges status;
  //  status.setSessionId(message->getSessionId());
  //  status.setUsercardId(message->getUsercardId());

    if (dbUsersCards) {
        int availabelFreeItems = db()->getFreeItemsCount(dbUsersCards);
        if (availabelFreeItems < message->receive()) {
            // return status false and new users statuses of this seller

            auto lastStatus = db()->
                    getUserCardData(message->getUser(), message->getCard());

//            status.setStatus(false);
//            status.addLastStatus(QSharedPointer<API::V3::UsersCards>::create(lastStatus));
//            if (!node()->sendData(&status, sender, &hdr)){
//                return false;
//            }
        }

    } else {
        dbUsersCards = db()->makeEmptyUsersCard();
        dbUsersCards->setUser(message->getUser());
        dbUsersCards->setCard(message->getCard());
    }

//    db()->saveSession(message->toObject(db()));

    dbUsersCards->setPurchasesNumber(dbUsersCards->getPurchasesNumber() + message->purchase());
    dbUsersCards->setTime(time(0));
    dbUsersCards->receive(message->receive());

    QByteArray neededCardId;
    if (!processCardStatusBase(QSharedPointer<V3::UsersCards>::create(dbUsersCards),
                               message->secret(), sender, hdr, neededCardId)) {
        return false;
    }

//    status.setNeededCard(neededCardId);

    auto lastStatus = db()->
            getUserCardData(message->getUser(), message->getCard());
//    status.addLastStatus(QSharedPointer<API::V3::UsersCards>::create(lastStatus));
//    status.setStatus(true);

//    return node()->sendData(&status, sender, &hdr);
}

bool ApiV3::accessValidation(const QSharedPointer<RC::Interfaces::iCard> &cardFromDB,
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

bool ApiV3::processContacts(const QSharedPointer<API::V3::UpdateContactData> &message,
                            const QH::AbstractNodeInfo * sender,
                            const QH::Header &hdr) {

    auto userKey = RCUtils::makeUserKey(message->userSecreet());

    if (userKey != message->getUserKey()) {
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

//    API::V3::UpdateContactDataResponce responce;
//    responce.setSuccessful(true);

//    return node()->sendData(&responce, sender, &hdr);
}

bool ApiV3::sendContacts(const Interfaces::iContacts& contact,
                         const QByteArray& secreet,
                         bool removeRequest,
                         QH::AbstractNodeInfo *dist,
                         const std::function<void(int err)>& cb) {


    auto request = QSharedPointer<RC::API::V3::UpdateContactData>::create(contact);
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

    return node()->sendData(&request, dist, nullptr);
}

bool ApiV3::sendUpdateCard(const QByteArray& cardId,
                           unsigned int version,
                           QH::AbstractNodeInfo *dist,
                           const std::function<void(int err)>& cb) {
    API::V3::CardUpdated request;
    request.setCardVersion(version);
    request.setCardId(cardId);

    return node()->sendData(&request, dist, nullptr);

}

bool ApiV3::changeUsersData(const QByteArray& sellerUserKey,
                            const QByteArray& cardId,
                            const QByteArray& userId,
                            unsigned int purchasesCount,
                            unsigned int receivedCount,
                            QH::AbstractNodeInfo *dist,
                            const std::function<void(int err, const QSharedPointer<Interfaces::iUsersCards>& currentState)>&  cb) {

    API::V3::ChangeUsersCards changes;

    changes.setPurchase(purchasesCount);
    changes.setReceive(receivedCount);
    changes.setSecret(db()->getSecret(sellerUserKey));


    _checkUserRequestHash.clear();

    auto packageHash = sendData(&changes, dist);
    _checkUserRequestHash += packageHash;

    return packageHash;

}

bool ApiV3::processSync(const QSharedPointer<V3::Sync> &message,
                        const QH::AbstractNodeInfo *sender,
                        const QH::Header &) {

    if (message->isRestrict()) {
        if (message->isContainsUsersDataInfo()) {
            db()->clearUsersData();
            for (const auto& usersData: message->usersCards().packData()) {
                db()->saveUsersCard(usersData->toObject(db()));
            }
        }

        if (message->isContainsPermisionsInfo()) {
            db()->clearPermisions();
            for (const auto& usersData: message->contacts().packData()) {
                db()->saveContact(usersData->toObject(db()));
            }
        }
    }

    if (message->isIncremental()) {
        if (message->isContainsUsersDataInfo()) {
            for (const auto& usersData: message->usersCards().packData()) {
                db()->saveUsersCard(usersData->toObject(db()));
            }
        }

        if (message->isContainsPermisionsInfo()) {
            for (const auto& usersData: message->contacts().packData()) {
                db()->saveContact(usersData->toObject(db()));
            }
        }
    }
}

}

}
