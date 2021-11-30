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
#include <CheatCard/api/api0/userscards.h>
#include <CheatCard/api/api0/card.h>
#include "CheatCard/api/api0/user.h"

#include <CheatCard/api/api1/restoredatarequest.h>
#include <CheatCard/api/api1/userscardsv1.h>

#include "CheatCard/nodeinfo.h"

#include <dbobjectsrequest.h>

#include <getsinglevalue.h>
#include <cmath>
#include <dbobjectsrequest.h>
#include "CheatCard/basenode.h"


namespace RC {

ApiV1::ApiV1(BaseNode *node): ApiV0(node) {

}

int ApiV1::version() const {
    return 1;
}

QH::ParserResult ApiV1::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                     const QH::Header &pkgHeader,
                                     const QH::AbstractNodeInfo *sender) {
    auto result = ApiV0::parsePackage(pkg, pkgHeader, sender);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<UsersCardsV1>>(this,
                                                             &ApiV1::processCardStatusV1,
                                                             pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<RestoreDataRequest>(this,
                                                &ApiV1::processRestoreDataRequest,
                                                pkg, sender, pkgHeader);

    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }


    return QH::ParserResult::NotProcessed;
}

bool ApiV1::processCardStatus(const QSharedPointer<QH::PKG::DataPack<UsersCards> > &,
                              const QH::AbstractNodeInfo *, const QH::Header &) {
    QuasarAppUtils::Params::log("The ApiV1 not support UsersCards pacakge v0, please send UsersCardsV1 or use ApiV0 parser",
                                QuasarAppUtils::Error);
    return false;
}

bool ApiV1::processCardStatusV1(const QSharedPointer<QH::PKG::DataPack<UsersCardsV1> > &cardStatuses,
                                const QH::AbstractNodeInfo *sender, const QH::Header &) {
    CardDataRequest request;

    for (const auto& cardStatus : cardStatuses->packData()) {
        Card userrquest;
        userrquest.setId(cardStatus->getCard());

        auto dbCard = db()->getObject(userrquest);

        if (!node()->cardValidation(dbCard, cardStatuses->customData())) {

            QuasarAppUtils::Params::log("Receive not signed cards seal");
            break;
        }

        if (!applayPurchases(cardStatus, sender)) {
            break;
        }

        bool hasUpdate = dbCard && dbCard->getCardVersion() < cardStatus->getCardVersion();

        if (!dbCard || hasUpdate) {
            request.push(cardStatus->getCard());
        }
    }

    if (request.getCardIds().size()) {

        if (!node()->sendData(&request, sender)) {
            QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);

            return false;
        }

        return true;
    }

    return node()->removeNode(sender->networkAddress());
}

bool ApiV1::applayPurchases(const QSharedPointer<UsersCards> &dbCard,
                            const QH::AbstractNodeInfo * sender) {

    return ApiV0::applayPurchases(dbCard, sender);
}

bool ApiV1::processCardRequest(const QSharedPointer<CardDataRequest> &cardrequest,
                               const QH::AbstractNodeInfo *sender, const QH::Header &) {

    QH::PKG::DataPack<Card> cards{};

    for (unsigned int cardId : cardrequest->getCardIds()) {
        auto card = node()->getCard(cardId);

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
    node()->getSignData(secret);
    cards.setCustomData(secret);

    if (!node()->sendData(&cards, sender)) {

        QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);
        return false;
    }

    return true;
}

bool ApiV1::processCardData(const QSharedPointer<QH::PKG::DataPack<Card>> &cards,
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

        if (!node()->cardValidation(db()->getObject(*card), cards->customData())) {

            QuasarAppUtils::Params::log("Receive not signed card",
                                        QuasarAppUtils::Error);
            break;
        }

        if (!db()->insertIfExistsUpdateObject(card)) {
            continue;
        }

        emit node()->sigCardReceived(card);
    }

    return node()->removeNode(sender->networkAddress());
}

bool ApiV1::processRestoreDataRequest(const QSharedPointer<RestoreDataRequest> &cardrequest,
                                      const QH::AbstractNodeInfo *sender, const QH::Header &) {


    QH::PKG::DataPack<UsersCardsV1> responce;

    unsigned int userID = User::makeId(cardrequest->userKey());

    QH::PKG::DBObjectsRequest<UsersCardsV1> request("UsersCards",
                                                    QString("user='%0'").arg(userID));

    auto result = db()->getObject(request);

    for (const auto &data : qAsConst(result->data())) {
        data->setCardVersion(node()->getCardVersion(data->getCard()));
        responce.push(data);
    }

    if (responce.isValid() && !node()->sendData(&responce, sender)) {
        return false;
    }

    auto cardsList = node()->getAllUserCards(cardrequest->userKey());
    QH::PKG::DataPack<Card> cardsPack(cardsList);

    if (cardsPack.isValid() && !node()->sendData(&cardsPack, sender)) {
        return false;
    }

    responce.setPackData({});

    for (const auto &card: qAsConst(cardsList)) {

        QH::PKG::DBObjectsRequest<UsersCardsV1> request("UsersCards",
                                                        QString("card='%0'").arg(card->cardId()));
        auto result = db()->getObject(request);

        for (const auto &data : qAsConst(result->data())) {
            responce.push(data);
        }
    }

    if (responce.isValid() && !node()->sendData(&responce, sender)) {
        return false;
    }

    return true;
}

bool ApiV1::processCardStatusRequest(const QSharedPointer<CardStatusRequest> &cardStatus,
                                     const QH::AbstractNodeInfo *sender, const QH::Header &) {

    auto sessionId = cardStatus->getSessionId();

    QString where = QString("id IN (SELECT usersCardsID FROM Sessions WHERE id = %0)").
            arg(sessionId);
    QH::PKG::DBObjectsRequest<UsersCardsV1> request("UsersCards", where);

    auto result = db()->getObject(request);
    if (!result || result->data().isEmpty()) {
        QuasarAppUtils::Params::log(QString("The session %0 is missing").
                                    arg(sessionId),
                                    QuasarAppUtils::Error);
        return false;
    }

    QH::PKG::DataPack<UsersCardsV1> responce;

    for (const auto &data : qAsConst(result->data())) {
        data->setCardVersion(node()->getCardVersion(data->getCard()));
        responce.push(data);
    }

    QByteArray secret;
    node()->getSignData(secret);
    responce.setCustomData(secret);

    if (!node()->sendData(&responce, sender)) {
        return false;
    }

    return true;
}

bool ApiV1::processSession(const QSharedPointer<Session> &session,
                           const QH::AbstractNodeInfo *sender,
                           const QH::Header &) {

    if (!session->isValid()) {
        return false;
    }

    session->setPrintError(false);
    db()->insertObject(session);

    CardStatusRequest requestData;

    requestData.setSessionId(session->getSessionId());

    return node()->sendData(&requestData, sender->networkAddress());

}


}
