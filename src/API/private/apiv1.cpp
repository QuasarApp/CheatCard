//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "apiv1.h"
#include "api0/carddatarequest.h"
#include "api0/cardstatusrequest.h"
#include <api0/session.h>
#include <api0/contacts.h>

#include <api1/restoredatarequest.h>
#include <api1/userscards.h>
#include <api1/card.h>

#include <dbobjectsrequest.h>

#include <getsinglevalue.h>
#include <cmath>
#include <dbobjectsrequest.h>

#include <rci/rcutils.h>


namespace RC {
namespace API {

ApiV1::ApiV1(const QSharedPointer<Interfaces::iDB>& dataBase, QH::AbstractNode* mainNode):
    ApiV0(dataBase, mainNode) {
}

void ApiV1::initSupportedCommands() {
    // for all nodes types.
    registerPackageType<QH::PKG::DataPack<API::V1::UsersCards>>();
    registerPackageType<QH::PKG::DataPack<API::V0::Card>>();
    registerPackageType<QH::PKG::DataPack<API::V1::Card>>();

    switch (static_cast<NodeType>(node()->nodeType())) {
    case NodeType::Visitor: {
        registerPackageType<QH::PKG::DataPack<API::V0::Contacts>>();
        break;
    }
    case NodeType::Seller: {
        registerPackageType<API::V0::CardStatusRequest>();
        registerPackageType<API::V0::CardDataRequest>();
        registerPackageType<QH::PKG::DataPack<API::V0::Contacts>>();
        break;
    }
    case NodeType::Server: {
        registerPackageType<API::V0::Session>();
        registerPackageType<API::V0::CardStatusRequest>();
        registerPackageType<QH::PKG::DataPack<API::V0::UsersCards>>();

        registerPackageType<API::V0::CardDataRequest>();
        registerPackageType<QH::PKG::DataPack<API::V0::Card>>();
        registerPackageType<API::V1::RestoreDataRequest>();
        break;
    }

    default:
        break;
    }
}

int ApiV1::version() const {
    return 1;
}

QH::ParserResult ApiV1::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                     const QH::Header &pkgHeader,
                                     QH::AbstractNodeInfo *sender) {

    auto result = commandHandler<API::V0::Session>(this, &ApiV1::processSession,
                                                   pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V0::CardStatusRequest>(this, &ApiV1::processCardStatusRequest,
                                                        pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<API::V1::UsersCards>>(this, &ApiV1::processCardStatus,
                                                                    pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V0::CardDataRequest>(this, &ApiV1::processCardRequest,
                                                      pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<API::V1::Card>>(this, &ApiV1::processCardData,
                                                              pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V1::RestoreDataRequest>(this,
                                                         &ApiV1::processRestoreDataRequest,
                                                         pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    return QH::ParserResult::NotProcessed;
}

void ApiV1::sendCardStatusRequest(long long userSession, QH::AbstractNodeInfo *dist) {
    V0::CardStatusRequest request;
    request.setSessionId(userSession);

    node()->sendData(&request, dist);
}

bool ApiV1::processCardStatus(const QSharedPointer<QH::PKG::DataPack<API::V1::UsersCards> > &cardStatuses,
                              const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {
    return processCardStatusImpl(*cardStatuses, sender, pkg);
}

bool ApiV1::processCardStatusImpl(const QH::PKG::DataPack<API::V1::UsersCards> &cardStatuses,
                                  const QH::AbstractNodeInfo *sender,
                                  const QH::Header &pkg) {
    V0::CardDataRequest request;

    for (const auto& cardStatus : cardStatuses.packData()) {
        auto dbCard = db()->getCard(cardStatus->getCard());

        if (!cardValidation(dbCard, cardStatuses.customData())) {

            QuasarAppUtils::Params::log(QString("Receive not signed cards seal."
                                                " Requester id = %0 Card: %1").
                                        arg(RCUtils::makeUserId(RCUtils::makeUserKey(cardStatuses.customData()))).
                                        arg(dbCard->toString()),
                                        QuasarAppUtils::Warning);
            break;
        }

        // Disable alert if this packge is ansver to restore request
        if (!applayPurchases(cardStatus, sender, pkg.triggerHash != _restoreDataPacakgeHash)) {
            break;
        }

        bool hasUpdate = dbCard && dbCard->getCardVersion() < cardStatus->getCardVersion();

        if (!dbCard || hasUpdate) {
            request.push(cardStatus->getCard());
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

bool ApiV1::processCardRequest(const QSharedPointer<API::V0::CardDataRequest> &cardrequest,
                               const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {

    QH::PKG::DataPack<API::V1::Card> cards{};

    QByteArray publicKey;
    for (unsigned int cardId : cardrequest->getCardIds()) {
        auto card = db()->getCard(cardId);

        if (!card) {
            QuasarAppUtils::Params::log(QString("Failed to find card with id: %0").
                                        arg(cardId),
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

bool ApiV1::processCardData(const QSharedPointer<QH::PKG::DataPack<API::V1::Card>> &cards,
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
        auto dbCard = db()->getCard(card->cardId());
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

void ApiV1::collectDataOfuser(const QByteArray& userKey, QH::PKG::DataPack<API::V1::UsersCards>& responce) {
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

bool ApiV1::processRestoreDataRequest(const QSharedPointer<API::V1::RestoreDataRequest> &cardrequest,
                                      const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {

    QH::PKG::DataPack<API::V1::UsersCards> responce;
    collectDataOfuser(cardrequest->userKey(), responce);

    if (responce.isValid()) {
        return node()->sendData(&responce, sender, &pkg);
    }

    return node()->removeNode(sender->networkAddress());
}

bool ApiV1::cardValidation(const QSharedPointer<RC::Interfaces::iCard> &cardFromDB,
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

bool ApiV1::processCardStatusRequest(const QSharedPointer<API::V0::CardStatusRequest> &cardStatus,
                                     const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {

    auto sessionId = cardStatus->getSessionId();
    const auto result = db()->getUsersCardsFromSession(sessionId);
    if (result.isEmpty()) {
        QuasarAppUtils::Params::log(QString("The session %0 is missing").
                                    arg(sessionId),
                                    QuasarAppUtils::Error);
        return false;
    }

    QH::PKG::DataPack<API::V1::UsersCards> responce;
    QByteArray publicKey;

    for (const auto &data : result) {
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

bool ApiV1::processSession(const QSharedPointer<API::V0::Session> &session,
                           const QH::AbstractNodeInfo *sender,
                           const QH::Header &pkg ) {

    if (!session->isValid()) {
        return false;
    }

    db()->saveSession(session->toObject(db()));

    V0::CardStatusRequest requestData;

    requestData.setSessionId(session->getSessionId());

    return node()->sendData(&requestData, sender->networkAddress(), &pkg);

}

void ApiV1::restoreOldDateRequest(const QByteArray &curentUserKey,
                                  QH::AbstractNodeInfo *dist) {

    V1::RestoreDataRequest request;
    request.setUserKey(curentUserKey);

    _restoreDataPacakgeHash = node()->sendData(&request, dist);
}

void ApiV1::restoreOneCardRequest(unsigned int cardId, QH::AbstractNodeInfo *dist) {
    V0::CardDataRequest request;
    request.push(cardId);

    node()->sendData(&request, dist);
}

bool ApiV1::sendContacts(const Interfaces::iContacts &,
                         const QByteArray &,
                         bool ,
                         QH::AbstractNodeInfo *) {
    QuasarAppUtils::Params::log(QString("Invoke not supported method : ApiV1::sendContacts"),
                                QuasarAppUtils::Warning);
    return false;
}

bool ApiV1::sendUpdateCard(unsigned int ,
                           unsigned int ,
                           QH::AbstractNodeInfo *) {
    QuasarAppUtils::Params::log(QString("Invoke not supported method : ApiV1::sendUpdateCard"),
                                QuasarAppUtils::Warning);
    return false;
}

bool ApiV1::changeUsersData(const QByteArray&,
                            unsigned int ,
                            unsigned int ,
                            unsigned long long ,
                            unsigned int ,
                            unsigned int ,
                            QH::AbstractNodeInfo *) {
    return false;
}


}
}
