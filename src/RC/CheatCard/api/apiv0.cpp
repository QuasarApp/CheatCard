//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "apiv0.h"
#include "CheatCard/api/api0/carddatarequest.h"
#include "CheatCard/api/api0/cardstatusrequest.h"
#include "CheatCard/api/api0/user.h"
#include <CheatCard/api/api0/session.h>
#include <CheatCard/api/api0/userscards.h>
#include <CheatCard/api/api0/card.h>

#include "CheatCard/nodeinfo.h"
#include "CheatCard/basenode.h"
#include <dbobjectsrequest.h>
#include <getsinglevalue.h>
#include <cmath>
#include <dbobjectsrequest.h>

namespace RC {

ApiV0::ApiV0(BaseNode *node) {
    _node = node;
}

ApiV0::~ApiV0() {
    if (_objectFactory)
        delete _objectFactory;
}

int ApiV0::version() const {
    return 0;
}

QH::ParserResult ApiV0::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                        const QH::Header &pkgHeader,
                                        const QH::AbstractNodeInfo *sender) {

    auto result = commandHandler<API::Session>(this, &ApiV0::processSession,
                                          pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::CardStatusRequest>(this, &ApiV0::processCardStatusRequest,
                                               pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<API::UsersCards>>(this, &ApiV0::processCardStatus,
                                        pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<API::CardDataRequest>(this, &ApiV0::processCardRequest,
                                             pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<API::Card>>(this, &ApiV0::processCardData,
                                  pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    return QH::ParserResult::NotProcessed;
}

void ApiV0::sendCardStatusRequest(long long userSession, QH::AbstractNodeInfo *dist) {
    API::CardStatusRequest request;

    auto senderInfo = static_cast<NodeInfo*>(dist);
    long long token = rand() * rand();
    senderInfo->setToken(token);
    request.setRequestToken(token);

    request.setSessionId(userSession);

    node()->sendData(&request, dist);
}

void ApiV0::sendSessions(const QHash<long long, QSharedPointer<API::Session> > &sessions,
                         QH::AbstractNodeInfo *dist) {

    for (const auto &session: qAsConst(sessions)) {
        node()->sendData(session.data(), dist);
    }
}

BaseNode *ApiV0::node() const {
    return _node;
}

bool ApiV0::processCardStatus(const QSharedPointer<QH::PKG::DataPack<API::UsersCards> > &cardStatuses,
                                 const QH::AbstractNodeInfo *sender, const QH::Header &) {

    QuasarAppUtils::Params::log(QString("processCardStatus: begin"), QuasarAppUtils::Info);
    API::CardDataRequest request;

    auto senderInfo = static_cast<const NodeInfo*>(sender);

    RequestToken tokens;
    tokens.fromBytes(cardStatuses->customData());
    if (senderInfo->token() != tokens.requestToken()) {
        QuasarAppUtils::Params::log("Receive not signed UsersCards!",
                                    QuasarAppUtils::Error);

        return false;
    }

    for (const auto& cardStatus : cardStatuses->packData()) {
        API::Card userrquest;
        userrquest.setId(cardStatus->getCard());

        if (!applayPurchases(cardStatus, sender)) {
            return false;
        }

        auto dbCard = db()->getObject(userrquest);
        bool hasUpdate = dbCard && dbCard->getCardVersion() < cardStatus->getCardVersion();

        if (!dbCard || hasUpdate) {
            request.push(cardStatus->getCard());
        }
    }

    if (request.getCardIds().size()) {
        request.setRequestToken(tokens.requestToken());
        request.setResponceToken(tokens.responceToken());

        if (!node()->sendData(&request, sender)) {
            QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);

            return false;
        }

        return true;
    }

    return node()->removeNode(sender->networkAddress());
}

bool ApiV0::applayPurchases(const QSharedPointer<API::UsersCards> &dbCard,
                               const QH::AbstractNodeInfo *) {

    if (!db()->insertIfExistsUpdateObject(dbCard)) {
        QuasarAppUtils::Params::log("Failed to update data", QuasarAppUtils::Error);
        return false;
    }

    emit node()->sigPurchaseWasSuccessful(dbCard);

    return true;

}

bool ApiV0::processCardRequest(const QSharedPointer<API::CardDataRequest> &cardrequest,
                                  const QH::AbstractNodeInfo *sender, const QH::Header &) {

    QuasarAppUtils::Params::log(QString("processCardRequest: begin"), QuasarAppUtils::Info);

    QH::PKG::DataPack<API::Card> cards{};
    auto senderInfo = static_cast<const NodeInfo*>(sender);

    if (senderInfo->token() != cardrequest->responceToken()) {
        QuasarAppUtils::Params::log("Receive not signed CardDataRequest!",
                                    QuasarAppUtils::Error);
        return false;
    }

    RequestToken tokens;
    tokens.setRequestToken(cardrequest->requestToken());
    tokens.setResponceToken(cardrequest->responceToken());


    for (unsigned int cardId : cardrequest->getCardIds()) {
        auto card = objectFactoryInstance()->getCard(cardId);

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

    if (!node()->sendData(&cards, sender)) {

        QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);
        return false;
    }

    return true;
}

bool ApiV0::processCardData(const QSharedPointer<QH::PKG::DataPack<API::Card>> &cards,
                               const QH::AbstractNodeInfo *sender, const QH::Header &) {

    QuasarAppUtils::Params::log(QString("processCardData: begin"), QuasarAppUtils::Info);

    auto senderInfo = static_cast<const NodeInfo*>(sender);

    RequestToken tokens;
    tokens.fromBytes(cards->customData());

    if (tokens.requestToken() != senderInfo->token()) {
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

        if (!db()->insertIfExistsUpdateObject(card)) {
            continue;
        }

        emit node()->sigCardReceived(card);
    }

    return node()->removeNode(sender->networkAddress());
}

QH::ISqlDBCache *ApiV0::db() const {
    if (auto nodePtr = dynamic_cast<BaseNode*>(node())) {
        return nodePtr->db();
    }

    return nullptr;
}

IAPIObjectsFactory *ApiV0::objectFactoryInstance() {

    if (_objectFactory)
        return _objectFactory;

    return _objectFactory = initObjectFactory();
}

IAPIObjectsFactory *ApiV0::initObjectFactory() const {
    return new APIObjectsFactoryV0(db());
}

bool ApiV0::processCardStatusRequest(const QSharedPointer<API::CardStatusRequest> &cardStatus,
                                        const QH::AbstractNodeInfo *sender, const QH::Header &) {

    QuasarAppUtils::Params::log(QString("processCardStatusRequest: begin"), QuasarAppUtils::Info);

    auto sessionId = cardStatus->getSessionId();

    QString where = QString("id IN (SELECT usersCardsID FROM Sessions WHERE id = %0)").
            arg(sessionId);
    QH::PKG::DBObjectsRequest<API::UsersCards> request("UsersCards", where);

    auto result = db()->getObject(request);
    if (!result || result->data().isEmpty()) {
        QuasarAppUtils::Params::log(QString("The session %0 is missing").
                                    arg(sessionId),
                                    QuasarAppUtils::Error);
        return false;
    }

    QH::PKG::DataPack<API::UsersCards> responce;

    RequestToken tokens;
    tokens.setRequestToken(cardStatus->requestToken());
    long long token = rand() * rand();
    tokens.setResponceToken(token);

    auto senderInfo = static_cast<NodeInfo*>(node()->getInfoPtr(sender->networkAddress()));
    senderInfo->setToken(token);
    responce.setCustomData(tokens.toBytes());

    for (const auto &data : qAsConst(result->data())) {
        data->setCardVersion(node()->getCardVersion(data->getCard()));
        responce.push(data);
    }

    if (!node()->sendData(&responce, sender)) {
        return false;
    }

    return true;
}

bool ApiV0::processSession(const QSharedPointer<API::Session> &session,
                              const QH::AbstractNodeInfo *sender, const QH::Header &) {

    if (!session->isValid()) {
        return false;
    }

    session->setPrintError(false);
    db()->insertObject(session);

    API::CardStatusRequest requestData;
    long long token = rand() * rand();

    requestData.setRequestToken(token);
    requestData.setSessionId(session->getSessionId());

    auto senderInfo = static_cast<NodeInfo*>(node()->getInfoPtr(sender->networkAddress()));
    senderInfo->setToken(token);

    return node()->sendData(&requestData, sender->networkAddress());
}


}
