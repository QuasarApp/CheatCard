//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "basenode.h"

#include <card.h>
#include <datastructures.h>
#include <userscards.h>

namespace RC {

BaseNode::BaseNode(QH::ISqlDBCache *db) {
    _db = db;
}

bool BaseNode::processCardStatus(const QSharedPointer<UsersCards> &cardStatus,
                                 const QH::AbstractNodeInfo *sender) {


    Card userrquest;
    userrquest.setId(cardStatus->getCard());

    auto dbCard = _db->getObject(userrquest);

    if (!dbCard) {
        CardDataRequest request;
        request.setCardId(cardStatus->getCard());

        if (!sendData(&request, sender->networkAddress())) {
            QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);

            return false;
        }
    }

    return applayPurchases(cardStatus, sender);
}

bool BaseNode::applayPurchases(const QSharedPointer<UsersCards> &dbCard,
                               const QH::AbstractNodeInfo *sender) {

    if (!_db->insertIfExistsUpdateObject(dbCard)) {
        QuasarAppUtils::Params::log("Failed to update data", QuasarAppUtils::Error);
        return false;
    }

    emit sigPurchaseWasSuccessful(dbCard);

    return removeNode(sender->networkAddress());

}

QSharedPointer<UsersCards>
BaseNode::getUserCardData(unsigned int userId, unsigned int cardId) {
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
                                  const QH::AbstractNodeInfo *sender) {

    auto card = getCard(cardrequest->getCardId());

    if (!sendData(card.data(), sender->networkAddress())) {

        QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);
        return false;
    }

    return true;
}

bool BaseNode::processCardData(const QSharedPointer<Card> &card,
                               const QH::AbstractNodeInfo *sender) {

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


}
