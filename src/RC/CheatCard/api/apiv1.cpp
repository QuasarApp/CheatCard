//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "apiv1.h"
#include "CheatCard/api/api0/carddatarequest.h"
#include "CheatCard/api/api0/cardstatusrequest.h"
#include <CheatCard/api/api0/session.h>
#include "CheatCard/api/api0/user.h"

#include <CheatCard/api/api1/restoredatarequest.h>
#include <CheatCard/api/api1/userscards.h>
#include <CheatCard/api/api1/card.h>

#include "CheatCard/nodeinfo.h"

#include <dbobjectsrequest.h>

#include <getsinglevalue.h>
#include <cmath>
#include <dbobjectsrequest.h>
#include "CheatCard/basenode.h"
#include <CheatCard/api/api0/contacts.h>


namespace RC {

ApiV1::ApiV1(BaseNode *node): ApiV0(node) {

}

int ApiV1::version() const {
    return 1;
}

QH::ParserResult ApiV1::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                     const QH::Header &pkgHeader,
                                     const QH::AbstractNodeInfo *sender) {

    auto result = commandHandler<API::Session>(this, &ApiV1::processSession,
                                               pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::CardStatusRequest>(this, &ApiV1::processCardStatusRequest,
                                                    pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<APIv1::UsersCards>>(this, &ApiV1::processCardStatus,
                                                                  pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::CardDataRequest>(this, &ApiV1::processCardRequest,
                                                  pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<APIv1::Card>>(this, &ApiV1::processCardData,
                                                            pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<APIv1::RestoreDataRequest>(this,
                                                       &ApiV1::processRestoreDataRequest,
                                                       pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    return QH::ParserResult::NotProcessed;
}

void ApiV1::sendCardStatusRequest(long long userSession, QH::AbstractNodeInfo *dist) {
    API::CardStatusRequest request;
    request.setSessionId(userSession);

    node()->sendData(&request, dist);
}

IAPIObjectsFactory *ApiV1::initObjectFactory() const {
    return new APIObjectsFactoryV1(db());
}

bool ApiV1::processCardStatus(const QSharedPointer<QH::PKG::DataPack<APIv1::UsersCards> > &cardStatuses,
                              const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {
    return processCardStatusImpl(*cardStatuses, sender, pkg);
}

bool ApiV1::processCardStatusImpl(const QH::PKG::DataPack<APIv1::UsersCards> &cardStatuses,
                              const QH::AbstractNodeInfo *sender,
                              const QH::Header &pkg) {
    API::CardDataRequest request;

    for (const auto& cardStatus : cardStatuses.packData()) {
        auto dbCard = objectFactoryInstance()->getCard(cardStatus->getCard());

        if (!cardValidation(dbCard, cardStatuses.customData())) {

            QuasarAppUtils::Params::log(QString("Receive not signed cards seal."
                                                " Requester id = %0 Card: %1").
                                        arg(API::User::makeId(API::User::makeKey(cardStatuses.customData()))).
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

bool ApiV1::processCardRequest(const QSharedPointer<API::CardDataRequest> &cardrequest,
                               const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {

    QH::PKG::DataPack<APIv1::Card> cards{};

    for (unsigned int cardId : cardrequest->getCardIds()) {
        auto card = objectFactoryInstance()->getCard(cardId).staticCast<APIv1::Card>();

        if (!card) {
            QuasarAppUtils::Params::log(QString("Failed to find card with id: %0").
                                        arg(cardId),
                                        QuasarAppUtils::Error);
            continue;
        }
        cards.push(card);
    }

    if (!cards.packData().size()) {
        QuasarAppUtils::Params::log(QString("Failed to find any cards "),
                                    QuasarAppUtils::Error);
        return false;
    }

    QByteArray secret;
    getSignData(secret);
    cards.setCustomData(secret);

    if (!node()->sendData(&cards, sender, &pkg)) {

        QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);
        return false;
    }

    return true;
}

bool ApiV1::processCardData(const QSharedPointer<QH::PKG::DataPack<APIv1::Card>> &cards,
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
        auto dbCard = db()->getObject(*card);
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

        if (!db()->insertIfExistsUpdateObject(card)) {
            continue;
        }

        emit node()->sigCardReceived(card);
    }

    return node()->removeNode(sender->networkAddress());
}

void RC::ApiV1::collectDataOfuser(const QByteArray& userKey, QH::PKG::DataPack<APIv1::UsersCards>& responce) {
    ;
    unsigned int userID = API::User::makeId(userKey);
    auto masterUser = node()->getMasterKeys(userKey);

    auto result = objectFactoryInstance()->getAllUserData(userID);

    for (const auto &data : qAsConst(result)) {
        data->setCardVersion(node()->getCardVersion(data->getCard()));
        responce.push(data.staticCast<APIv1::UsersCards>());
    }

    const auto datalist = objectFactoryInstance()->getAllUserCardsData(userKey, masterUser);
    for (const auto& item: datalist) {
        item->setCardVersion(node()->getCardVersion(item->getCard()));
        responce.push(item.staticCast<APIv1::UsersCards>());
    }

    return;
}

bool ApiV1::processRestoreDataRequest(const QSharedPointer<APIv1::RestoreDataRequest> &cardrequest,
                                      const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {

    QH::PKG::DataPack<APIv1::UsersCards> responce;
    collectDataOfuser(cardrequest->userKey(), responce);

    if (responce.isValid()) {
        return node()->sendData(&responce, sender, &pkg);
    }

    return node()->removeNode(sender->networkAddress());
}

bool ApiV1::cardValidation(const QSharedPointer<API::Card> &cardFromDB,
                           const QByteArray &ownerSecret) const {

    switch (NodeTypeHelper::getBaseType(node()->nodeType())) {
    case NodeType::Server: {
        if (!cardFromDB)
            return true;

        auto signature = cardFromDB->ownerSignature();

        if (signature.isEmpty()) {
            return true;
        }

        auto ownerSignature =  API::User::makeKey(ownerSecret);

        return signature == ownerSignature;
    }

    default: {};
    }

    return true;
}

void ApiV1::getSignData(QByteArray &data) const {

    switch (NodeTypeHelper::getBaseType(node()->nodeType())) {
    case NodeType::Seller: {
        auto nodePtr = node()->currentUser();
        if (nodePtr)
            data = nodePtr->secret();

        break;
    }

    default: {};
    }
}

bool ApiV1::processCardStatusRequest(const QSharedPointer<API::CardStatusRequest> &cardStatus,
                                     const QH::AbstractNodeInfo *sender, const QH::Header &pkg) {

    auto sessionId = cardStatus->getSessionId();

    QString where = QString("id IN (SELECT usersCardsID FROM Sessions WHERE id = %0)").
            arg(sessionId);
    QH::PKG::DBObjectsRequest<APIv1::UsersCards> request("UsersCards", where);

    auto result = db()->getObject(request);
    if (!result || result->data().isEmpty()) {
        QuasarAppUtils::Params::log(QString("The session %0 is missing").
                                    arg(sessionId),
                                    QuasarAppUtils::Error);
        return false;
    }

    QH::PKG::DataPack<APIv1::UsersCards> responce;

    for (const QSharedPointer<APIv1::UsersCards> &data : qAsConst(result->data())) {
        data->setCardVersion(node()->getCardVersion(data->getCard()));
        responce.push(data);
    }

    QByteArray secret;
    getSignData(secret);
    responce.setCustomData(secret);

    if (!node()->sendData(&responce, sender, &pkg)) {
        return false;
    }

    sessionProcessed(sessionId);

    return true;
}

bool ApiV1::processSession(const QSharedPointer<API::Session> &session,
                           const QH::AbstractNodeInfo *sender,
                           const QH::Header &pkg ) {

    if (!session->isValid()) {
        return false;
    }

    session->setPrintError(false);
    db()->insertObject(session);

    API::CardStatusRequest requestData;

    requestData.setSessionId(session->getSessionId());

    return node()->sendData(&requestData, sender->networkAddress(), &pkg);

}

void RC::ApiV1::restoreOldDateRequest(const QByteArray &curentUserKey,
                                      QH::AbstractNodeInfo *dist) {

    APIv1::RestoreDataRequest request;
    request.setUserKey(curentUserKey);

    _restoreDataPacakgeHash = node()->sendData(&request, dist);
}

void ApiV1::restoreOneCardRequest(unsigned int cardId, QH::AbstractNodeInfo *dist) {
    API::CardDataRequest request;
    request.push(cardId);

    node()->sendData(&request, dist);
}


}
