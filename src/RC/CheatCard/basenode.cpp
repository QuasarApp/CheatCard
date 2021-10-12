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

    result = commandHandler<UsersCards>(this, &BaseNode::processCardStatus,
                                        pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<CardDataRequest>(this, &BaseNode::processCardRequest,
                                             pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<Card>(this, &BaseNode::processCardData,
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

int RC::BaseNode::getCardFreeIndex(unsigned int cardId) const {
    QH::PKG::GetSingleValue request({"Cards", cardId}, "freeIndex");
    auto result = _db->getObject(request);

    if (!result) {
        return 0;
    }

    return result->value().toUInt();
}

QString BaseNode::libVersion() const {
    return CHEAT_CARD_VERSION;
}

bool BaseNode::processCardStatus(const QSharedPointer<UsersCards> &cardStatus,
                                 const QH::AbstractNodeInfo *sender, const QH::Header &) {

    QuasarAppUtils::Params::log(QString("processCardStatus: begin"), QuasarAppUtils::Info);

    Card userrquest;
    userrquest.setId(cardStatus->getCard());

    if (!applayPurchases(cardStatus, sender)) {
        return false;
    }

    auto dbCard = _db->getObject(userrquest);

    if (!dbCard) {
        CardDataRequest request;
        request.setCardId(cardStatus->getCard());
        unsigned long long token = rand() * rand();
        request.setRequestToken(token);

        auto senderInfo = static_cast<NodeInfo*>(getInfoPtr(sender->networkAddress()));
        senderInfo->setToken(token);

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

    auto card = getCard(cardrequest->getCardId());

    if (!card) {
        QuasarAppUtils::Params::log(QString("Failed to find card with id: %0").
                                    arg(cardrequest->getCardId()),
                                    QuasarAppUtils::Error);
        return false;
    }

    card->setRequestToken(cardrequest->requestToken());

    if (!sendData(card.data(), sender)) {

        QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);
        return false;
    }

    return true;
}

bool BaseNode::processCardData(const QSharedPointer<Card> &card,
                               const QH::AbstractNodeInfo *sender, const QH::Header &) {

    QuasarAppUtils::Params::log(QString("processCardData: begin"), QuasarAppUtils::Info);

    auto senderInfo = static_cast<const NodeInfo*>(sender);

    if (!card->isValid())
        return false;

    if (card->requestToken() != senderInfo->token()) {
        QuasarAppUtils::Params::log("Receive not requested responce!");
        return false;
    }

    if (!_db->insertIfExistsUpdateObject(card)) {
        return false;
    }

    emit sigCardReceived(card);

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

    for (const auto &data : qAsConst(result->data())) {
        if (!sendData(data.data(), sender)) {
            return false;
        }
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
    requestData.setSessionId(session->getSessionId());

    return sendData(&requestData, sender->networkAddress());
}


}
