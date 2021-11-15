//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "apiv1.h"
#include "CheatCard/carddatarequest.h"
#include "CheatCard/cardstatusrequest.h"
#include "CheatCard/nodeinfo.h"
#include "CheatCard/user.h"

#include <CheatCard/card.h>
#include <dbobjectsrequest.h>
#include <CheatCard/session.h>
#include <CheatCard/userscards.h>
#include <getsinglevalue.h>
#include <cmath>
#include <dbobjectsrequest.h>
#include "CheatCard/basenode.h"

namespace RC {

ApiV1::ApiV1(BaseNode *node): ApiV0(node) {

}

bool ApiV1::processCardStatus(const QSharedPointer<QH::PKG::DataPack<UsersCards> > &cardStatuses,
                                   const QH::AbstractNodeInfo *sender, const QH::Header &) {

    CardDataRequest request;

    for (const auto& cardStatus : cardStatuses->packData()) {
        Card userrquest;
        userrquest.setId(cardStatus->getCard());

        auto dbCard = db()->getObject(userrquest);

        if (node()->cardValidation(dbCard, cardStatuses->customData())) {

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

        if (node()->cardValidation(card, cards->customData())) {

            QuasarAppUtils::Params::log("Receive not signed card");
            break;
        }

        if (!db()->insertIfExistsUpdateObject(card)) {
            continue;
        }

        emit node()->sigCardReceived(card);
    }

    return node()->removeNode(sender->networkAddress());
}

bool ApiV1::processCardStatusRequest(const QSharedPointer<CardStatusRequest> &cardStatus,
                                          const QH::AbstractNodeInfo *sender, const QH::Header &) {

    auto sessionId = cardStatus->getSessionId();

    QString where = QString("id IN (SELECT usersCardsID FROM Sessions WHERE id = %0)").
            arg(sessionId);
    QH::PKG::DBObjectsRequest<UsersCards> request("UsersCards", where);

    auto result = db()->getObject(request);
    if (!result || result->data().isEmpty()) {
        QuasarAppUtils::Params::log(QString("The session %0 is missing").
                                    arg(sessionId),
                                    QuasarAppUtils::Error);
        return false;
    }

    QH::PKG::DataPack<UsersCards> responce;

    for (const auto &data : qAsConst(result->data())) {
        data->setCardVersion(node()->getCardVersion(data->getCard()));
        responce.push(data);
    }

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
