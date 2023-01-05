//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "apiv0.h"
#include "api0/carddatarequest.h"
#include "api0/cardstatusrequest.h"
#include "params.h"
#include <api0/session.h>
#include <api0/userscards.h>
#include <api0/card.h>
#include <rci/rcutils.h>

#include <dbobjectsrequest.h>
#include <getsinglevalue.h>
#include <cmath>
#include <dbobjectsrequest.h>

namespace RC {
namespace API {

ApiV0::ApiV0(const QSharedPointer<Interfaces::iDB>& dataBase, QH::AbstractNode* mainNode):
    API::APIBase(dataBase, mainNode){}

ApiV0::~ApiV0() {
}

int ApiV0::version() const {
    return 0;
}

QH::ParserResult ApiV0::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                     const QH::Header &pkgHeader,
                                     QH::AbstractNodeInfo *sender) {

    auto result = commandHandler<API::V0::Session>(this, &ApiV0::processSession,
                                               pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V0::CardStatusRequest>(this, &ApiV0::processCardStatusRequest,
                                                    pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<API::V0::UsersCards>>(this, &ApiV0::processCardStatus,
                                                                pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::V0::CardDataRequest>(this, &ApiV0::processCardRequest,
                                                  pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<API::V0::Card>>(this, &ApiV0::processCardData,
                                                          pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    return QH::ParserResult::NotProcessed;
}

void ApiV0::sendCardStatusRequest(long long userSession, QH::AbstractNodeInfo *dist) {
    API::V0::CardStatusRequest request;

    auto& info = _extraData[dist->networkAddress()];
    info.token = rand() * rand();
    request.setRequestToken(info.token);


    request.setSessionId(userSession);

    node()->sendData(&request, dist);
}

void ApiV0::sendSessions(const QHash<long long, QSharedPointer<Interfaces::iSession> > &sessions,
                         QH::AbstractNodeInfo *dist) {

    for (const auto &session: qAsConst(sessions)) {
        API::V0::Session obj(session);
        node()->sendData(&obj, dist);
    }
}

void ApiV0::initSupportedCommands() {
    // not supported anymore
}

bool ApiV0::processCardStatus(const QSharedPointer<QH::PKG::DataPack<API::V0::UsersCards> > &cardStatuses,
                              const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    QuasarAppUtils::Params::log(QString("processCardStatus: begin"), QuasarAppUtils::Info);
    API::V0::CardDataRequest request;

    auto& senderInfo = _extraData[sender->networkAddress()];

    V0::RequestToken tokens;
    tokens.fromBytes(cardStatuses->customData());
    if (senderInfo.token != tokens.requestToken()) {
        QuasarAppUtils::Params::log("Receive not signed UsersCards!",
                                    QuasarAppUtils::Error);

        return false;
    }

    for (const auto& cardStatus : cardStatuses->packData()) {
        V0::Card userrquest;
        userrquest.setId(cardStatus->getCard());

        if (!applayPurchases(cardStatus, sender)) {
            return false;
        }

        auto dbCard = db()->getCard(cardStatus->getCard());
        bool hasUpdate = dbCard && dbCard->getCardVersion() < cardStatus->getCardVersion();

        if (!dbCard || hasUpdate) {
            request.push(cardStatus->getCard());
        }
    }

    if (request.getCardIds().size()) {
        request.setRequestToken(tokens.requestToken());
        request.setResponceToken(tokens.responceToken());

        if (!node()->sendData(&request, sender, &hdr)) {
            QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);

            return false;
        }

        return true;
    }

    return node()->removeNode(sender->networkAddress());
}

bool ApiV0::applayPurchases(const QSharedPointer<API::V0::UsersCards> &dbCard,
                            const QH::AbstractNodeInfo *, bool alert) {

    auto usersCard = dbCard->toObject(db());
    if (!db()->saveUsersCard(usersCard)) {
        QuasarAppUtils::Params::log("Failed to update data", QuasarAppUtils::Error);
        return false;
    }

    emit sigPurchaseWasSuccessful(usersCard, alert);

    return true;

}

bool ApiV0::processCardRequest(const QSharedPointer<API::V0::CardDataRequest> &cardrequest,
                               const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    QuasarAppUtils::Params::log(QString("processCardRequest: begin"), QuasarAppUtils::Info);

    QH::PKG::DataPack<API::V0::Card> cards{};
    auto& senderInfo = _extraData[sender->networkAddress()];

    if (senderInfo.token != cardrequest->responceToken()) {
        QuasarAppUtils::Params::log("Receive not signed CardDataRequest!",
                                    QuasarAppUtils::Error);
        return false;
    }

    V0::RequestToken tokens;
    tokens.setRequestToken(cardrequest->requestToken());
    tokens.setResponceToken(cardrequest->responceToken());


    for (unsigned int cardId : cardrequest->getCardIds()) {
        auto card = db()->getCard(cardId);

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

    cards.setCustomData(tokens.toBytes());

    if (!node()->sendData(&cards, sender, &hdr)) {

        QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);
        return false;
    }

    return true;
}

bool ApiV0::processCardData(const QSharedPointer<QH::PKG::DataPack<API::V0::Card>> &cards,
                            const QH::AbstractNodeInfo *sender, const QH::Header &) {

    QuasarAppUtils::Params::log(QString("processCardData: begin"), QuasarAppUtils::Info);

    auto& senderInfo = _extraData[sender->networkAddress()];

    V0::RequestToken tokens;
    tokens.fromBytes(cards->customData());

    if (tokens.requestToken() != senderInfo.token) {
        QuasarAppUtils::Params::log("Receive not signed Card!",
                                    QuasarAppUtils::Error);
        return false;
    }

    for (const auto& card : qAsConst(cards->packData())) {
        if (!card->isValid()) {
            QuasarAppUtils::Params::log("Received invalid card data!",
                                        QuasarAppUtils::Error);
            continue;
        }

        auto cardObj = card->toObject(db());
        if (!db()->saveCard(cardObj)) {
            continue;
        }

        emit sigCardReceived(cardObj);
    }

    return node()->removeNode(sender->networkAddress());
}

void ApiV0::sessionProcessed(unsigned long long sessionId) {
    switch (static_cast<NodeType>(node()->nodeType())) {
    case NodeType::Server: {
        db()->deleteSessuon(sessionId);
        break;
    }

    default: {};
    }
}

void ApiV0::initializeSupportedCommands() {
    // not supported any more.
}

bool ApiV0::processCardStatusRequest(const QSharedPointer<API::V0::CardStatusRequest> &cardStatus,
                                     const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    QuasarAppUtils::Params::log(QString("processCardStatusRequest: begin"), QuasarAppUtils::Info);
    auto sessionId = cardStatus->getSessionId();
    const auto result = db()->getUsersCardsFromSession(cardStatus->getSessionId());
    if (result.isEmpty()) {
        QuasarAppUtils::Params::log(QString("The session %0 is missing").
                                    arg(sessionId),
                                    QuasarAppUtils::Error);
        return false;
    }

    QH::PKG::DataPack<API::V0::UsersCards> responce;

    V0::RequestToken tokens;
    tokens.setRequestToken(cardStatus->requestToken());
    long long token = rand() * rand();
    tokens.setResponceToken(token);

    auto& senderInfo = _extraData[sender->networkAddress()];

    senderInfo.token = token;
    responce.setCustomData(tokens.toBytes());

    for (const auto &data : result) {
        data->setCardVersion(db()->getCardVersion(data->getCard()));
        responce.push(data);
    }

    if (!node()->sendData(&responce, sender, &hdr)) {
        return false;
    }

    sessionProcessed(sessionId);

    return true;
}

bool ApiV0::processSession(const QSharedPointer<API::V0::Session> &session,
                           const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    if (!session->isValid()) {
        return false;
    }

    db()->saveSession(session->toObject(db()));

    V0::CardStatusRequest requestData;
    long long token = rand() * rand();

    requestData.setRequestToken(token);
    requestData.setSessionId(session->getSessionId());

    auto& senderInfo = _extraData[sender->networkAddress()];
    senderInfo.token = token;

    return node()->sendData(&requestData, sender->networkAddress(), &hdr);
}

}
}
