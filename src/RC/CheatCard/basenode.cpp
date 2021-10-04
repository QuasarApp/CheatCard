//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "basenode.h"

#include <CheatCard/card.h>
#include <CheatCard/datastructures.h>
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

int BaseNode::getFreeItemsCount(unsigned int userId,
                                unsigned int cardId) const {
    return getFreeItemsCount(getUserCardData(userId, cardId));
}

int BaseNode::getFreeItemsCount(const QSharedPointer<UsersCards> &inputData) const {

    QH::PKG::GetSingleValue request({"Cards", inputData->getCard()}, "freeIndex");
    auto result = _db->getObject(request);

    if (!result) {
        return 0;
    }

    unsigned int freeIndex = result->value().toUInt();

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

QString BaseNode::libVersion() const {
    return CHEAT_CARD_VERSION;
}

bool BaseNode::processCardStatus(const QSharedPointer<UsersCards> &cardStatus,
                                 const QH::AbstractNodeInfo *sender, const QH::Header &) {


    Card userrquest;
    userrquest.setId(cardStatus->getCard());

    if (!applayPurchases(cardStatus, sender)) {
        return false;
    }

    auto dbCard = _db->getObject(userrquest);

    if (!dbCard) {
        CardDataRequest request;
        request.setCardId(cardStatus->getCard());

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

    auto card = getCard(cardrequest->getCardId());

    if (!sendData(card.data(), sender)) {

        QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);
        return false;
    }

    return true;
}

bool BaseNode::processCardData(const QSharedPointer<Card> &card,
                               const QH::AbstractNodeInfo *sender, const QH::Header &) {

    if (!card->isValid())
        return false;

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
