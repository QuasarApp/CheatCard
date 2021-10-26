//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "basenode.h"
#include "carddatarequest.h"
#include "cardstatusrequest.h"
#include "nodeinfo.h"

#include <CheatCard/card.h>
#include <dbobjectsrequest.h>
#include <CheatCard/session.h>
#include <CheatCard/userscards.h>
#include <getsinglevalue.h>
#include <cmath>

namespace RC {

BaseNode::BaseNode(QH::ISqlDBCache *db) {
    _db = db;
}

QH::ParserResult BaseNode::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                        const QH::Header &pkgHeader,
                                        const QH::AbstractNodeInfo *sender) {

    auto parentResult = AbstractNode::parsePackage(pkg, pkgHeader, sender);
    if (parentResult != QH::ParserResult::NotProcessed) {
        return parentResult;
    }

    auto result = commandHandler<Session>(this, &BaseNode::processSession,
                                          pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<CardStatusRequest>(this, &BaseNode::processCardStatusRequest,
                                               pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<UsersCards>>(this, &BaseNode::processCardStatus,
                                        pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<CardDataRequest>(this, &BaseNode::processCardRequest,
                                             pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<QH::PKG::DataPack<Card>>(this, &BaseNode::processCardData,
                                  pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    return QH::ParserResult::NotProcessed;
}

QH::AbstractNodeInfo *BaseNode::createNodeInfo(QAbstractSocket *socket,
                                               const QH::HostAddress *clientAddress) const {
    return new NodeInfo(socket, clientAddress);
}

int BaseNode::getFreeItemsCount(unsigned int userId,
                                unsigned int cardId) const {
    return getFreeItemsCount(getUserCardData(userId, cardId));
}

int BaseNode::getFreeItemsCount(const QSharedPointer<UsersCards> &inputData) const {
    unsigned int freeIndex = getCardFreeIndex(inputData->getCard());
    return getFreeItemsCount(inputData, freeIndex);
}

int BaseNode::getFreeItemsCount(const QSharedPointer<UsersCards> &inputData,
                                unsigned int freeIndex) const {
    if (freeIndex <= 0)
        return 0;

    int freeItems = std::floor(inputData->getPurchasesNumber() /
                               static_cast<double>(freeIndex)) -
            inputData->getReceived();

    return freeItems;
}

int BaseNode::getCountOfReceivedItems(unsigned int userId,
                                      unsigned int cardId) {
    return getUserCardData(userId, cardId)->getReceived();
}

int RC::BaseNode::getCardFreeIndex(unsigned int cardId) const {
    QH::PKG::GetSingleValue request({"Cards", cardId}, "freeIndex");
    auto result = _db->getObject(request);

    if (!result) {
        return 0;
    }

    return result->value().toUInt();
}

QString BaseNode::libVersion() {
    return CHEAT_CARD_VERSION;
}

bool BaseNode::processCardStatus(const QSharedPointer<QH::PKG::DataPack<UsersCards> > &cardStatuses,
                                 const QH::AbstractNodeInfo *sender, const QH::Header &) {

    QuasarAppUtils::Params::log(QString("processCardStatus: begin"), QuasarAppUtils::Info);
    CardDataRequest request;

    auto senderInfo = static_cast<const NodeInfo*>(sender);

    RequestToken tokens;
    tokens.fromBytes(cardStatuses->customData());
    if (senderInfo->token() != tokens.requestToken()) {
        QuasarAppUtils::Params::log("Receive not signed UsersCards!",
                                    QuasarAppUtils::Error);

        return false;
    }

    for (const auto& cardStatus : cardStatuses->packData()) {
        Card userrquest;
        userrquest.setId(cardStatus->getCard());

        if (!applayPurchases(cardStatus, sender)) {
            return false;
        }

        auto dbCard = _db->getObject(userrquest);

        if (!dbCard) {
            request.push(cardStatus->getCard());
        }
    }

    if (request.getCardIds().size()) {
        request.setRequestToken(tokens.requestToken());
        request.setResponceToken(tokens.responceToken());

        if (!sendData(&request, sender)) {
            QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);

            return false;
        }

        return true;
    }

    return removeNode(sender->networkAddress());
}

bool BaseNode::applayPurchases(const QSharedPointer<UsersCards> &dbCard,
                               const QH::AbstractNodeInfo *) {

    if (!_db->insertIfExistsUpdateObject(dbCard)) {
        QuasarAppUtils::Params::log("Failed to update data", QuasarAppUtils::Error);
        return false;
    }

    emit sigPurchaseWasSuccessful(dbCard);

    return true;

}

QSharedPointer<UsersCards>
BaseNode::getUserCardData(unsigned int userId, unsigned int cardId) const {
    UsersCards request;
    request.setCard(cardId);
    request.setUser(userId);

    auto purches = _db->getObject(request);
    return purches;
}

QSharedPointer<Card> BaseNode::getCard(unsigned int cardId) {
    Card request;
    request.setId(cardId);

    return _db->getObject(request);
}

bool BaseNode::processCardRequest(const QSharedPointer<CardDataRequest> &cardrequest,
                                  const QH::AbstractNodeInfo *sender, const QH::Header &) {

    QuasarAppUtils::Params::log(QString("processCardRequest: begin"), QuasarAppUtils::Info);

    QH::PKG::DataPack<Card> cards{};
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
        auto card = getCard(cardId);

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

    if (!sendData(&cards, sender)) {

        QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);
        return false;
    }

    return true;
}

bool BaseNode::processCardData(const QSharedPointer<QH::PKG::DataPack<Card>> &cards,
                               const QH::AbstractNodeInfo *sender, const QH::Header &) {

    QuasarAppUtils::Params::log(QString("processCardData: begin"), QuasarAppUtils::Info);

    auto senderInfo = static_cast<const NodeInfo*>(sender);

    RequestToken tokens;
    tokens.fromBytes(cards->customData());

    if (tokens.requestToken() != senderInfo->token()) {
        QuasarAppUtils::Params::log("Receive not signed Card!",
                                    QuasarAppUtils::Error);
        return false;;
    }

    for (const auto& card : qAsConst(cards->packData())) {
        if (!card->isValid()) {
            QuasarAppUtils::Params::log("Received invalid card data!",
                                        QuasarAppUtils::Error);
            continue;
        }

        if (!_db->insertIfExistsUpdateObject(card)) {
            continue;
        }

        emit sigCardReceived(card);
    }

    return removeNode(sender->networkAddress());
}

QH::ISqlDBCache *BaseNode::db() const {
    return _db;
}

bool BaseNode::processCardStatusRequest(const QSharedPointer<CardStatusRequest> &cardStatus,
                                        const QH::AbstractNodeInfo *sender, const QH::Header &) {

    QuasarAppUtils::Params::log(QString("processCardStatusRequest: begin"), QuasarAppUtils::Info);

    auto sessionId = cardStatus->getSessionId();

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

    QH::PKG::DataPack<UsersCards> responce;

    RequestToken tokens;
    tokens.setRequestToken(cardStatus->requestToken());
    long long token = rand() * rand();
    tokens.setResponceToken(token);

    auto senderInfo = static_cast<NodeInfo*>(getInfoPtr(sender->networkAddress()));
    senderInfo->setToken(token);
    responce.setCustomData(tokens.toBytes());

    for (const auto &data : qAsConst(result->data())) {
        responce.push(data);
    }

    if (!sendData(&responce, sender)) {
        return false;
    }

    return true;
}

bool BaseNode::processSession(const QSharedPointer<Session> &session,
                              const QH::AbstractNodeInfo *sender, const QH::Header &) {

    if (!session->isValid()) {
        return false;
    }

    session->setPrintError(false);
    db()->insertObject(session);

    CardStatusRequest requestData;
    long long token = rand() * rand();

    requestData.setRequestToken(token);
    requestData.setSessionId(session->getSessionId());

    auto senderInfo = static_cast<NodeInfo*>(getInfoPtr(sender->networkAddress()));
    senderInfo->setToken(token);

    return sendData(&requestData, sender->networkAddress());
}


}
