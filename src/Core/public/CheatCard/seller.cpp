//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "basenode.h"
#include "params.h"
#include "rci/core/iapi.h"
#include "usersnames.h"
#include "seller.h"
#include "nodeinfo.h"
#include "userheader.h"
#include <rci/rcutils.h>
#include "api/apibase.h"
#include <functional>

namespace RC {

Seller::Seller(const QSharedPointer<Interfaces::iDB> &db): BaseNode(db) {

}

bool Seller::incrementPurchases(const QSharedPointer<Interfaces::iUsersCards> &usersCardsData,
                                int purchasesCount) {

    if (!usersCardsData)
        return false;

    if (purchasesCount < 0)
        return false;

    usersCardsData->setPurchasesNumber(usersCardsData->getPurchasesNumber() + purchasesCount);

    if (!db()->saveUsersCard(usersCardsData)) {
        return false;
    }

    emit sigPurchaseWasSuccessful(usersCardsData, true);

    return true;
}

QSharedPointer<Interfaces::iSession>
Seller::prepareSession(const UserHeader &userHeaderData,
                       unsigned int cardId) const {

    auto session = db()->makeEmptySession();
    session->setSessionId(userHeaderData.getSessionId());
    session->setUsercardId(RCUtils::makeUsersCardsId(userHeaderData.getUserId(), cardId));

    if (!db()->saveSession(session)) {
        return nullptr;
    }

    return session;
}

void RC::Seller::updateUsersData(const UserHeader &userHeaderData)
{
    auto dbUser = db()->getUser(userHeaderData.getUserId());

    if (dbUser && !dbUser->secret().isEmpty()) {
        return;
    }

    if (!dbUser) {
        userHeaderData.toUser(dbUser);
    }

    if (dbUser->name().isEmpty()) {
        dbUser->setName(UsersNames::randomUserName());
    }

    if (!db()->saveUser(dbUser)) {
        QuasarAppUtils::Params::log("Failed to update user data", QuasarAppUtils::Warning);

    }
}

QSharedPointer<Interfaces::iUsersCards>
Seller::prepareData(const UserHeader &userHeaderData,
                    unsigned int cardId) {

    if (!userHeaderData.isValid())
        return nullptr;

    updateUsersData(userHeaderData);

    auto userCardsData = db()->getUserCardData(userHeaderData.getUserId(), cardId);
    if (!userCardsData) {
        userCardsData = db()->makeEmptyUsersCard();
        userCardsData->setUser(userHeaderData.getUserId());
        userCardsData->setPurchasesNumber(0);
        userCardsData->setCard(cardId);
    }

    return userCardsData;
}

bool Seller::sendDataPrivate(const QString &domain, int port,
                             sendPrivateCB action) {
    auto wrap = [this, action](QH::AbstractNodeInfo *node) {
        auto api = getApi(node);
        if (api && action) {
            action(api, node);
        }
    };

    if (domain.isEmpty()) {
        return addNode(getServerHost(), port, wrap,
                       QH::NodeCoonectionStatus::Confirmed);
    }

    return addNode(domain, port, wrap,
                   QH::NodeCoonectionStatus::Confirmed);
}

bool Seller::incrementPurchase(const QSharedPointer<UserHeader> &userHeaderData,
                               unsigned int cardId, int purchasesCount,
                               const QString &domain, int port) {

    auto action = [this, userHeaderData, cardId, purchasesCount]
        (const QSharedPointer<Interfaces::iAPI>& api, QH::AbstractNodeInfo *dist) {
            return api->changeUsersData(currentUser()->getKey(),
                                        cardId,
                                        userHeaderData->getUserId(),
                                        userHeaderData->getSessionId(),
                                        purchasesCount, 0,
                                        dist);
        };

    return sendDataPrivate(domain, port, action);
}

bool Seller::setPurchase(const UserHeader &userHeaderData,
                         unsigned int cardId, int purchasesCount,
                         const QString &domain, int port) {

    // code for old api
    auto session = prepareSession(userHeaderData, cardId);
    auto usersCardsData = prepareData(userHeaderData, cardId);
    if (!usersCardsData)
        return false;

    usersCardsData->setPurchasesNumber(purchasesCount);

    if (!db()->saveUsersCard(usersCardsData)) {
        return false;
    }

    auto action = [session](const QSharedPointer<Interfaces::iAPI>& api,
                            QH::AbstractNodeInfo *dist) {
        api->sendSessions({{session->getSessionId(), session}}, dist);
        return true;
    };

    return sendDataPrivate(domain, port, action);
}

bool Seller::sentDataToServerReceive(const QSharedPointer<UserHeader> &userHeaderData,
                                     unsigned int cardId,
                                     int receiveCount,
                                     const QString &domain,
                                     int port) {

    auto action = [userHeaderData, cardId, receiveCount, this]
        (const QSharedPointer<Interfaces::iAPI>& api, QH::AbstractNodeInfo *dist) {
            return api->changeUsersData(currentUser()->getKey(),
                                        cardId,
                                        userHeaderData->getUserId(),
                                        userHeaderData->getSessionId(),
                                        0, receiveCount,
                                        dist);
        };

    return sendDataPrivate(domain, port, action);
}

bool Seller::cardUpdated(unsigned int cardId, unsigned int version,
                         const QString &domain,
                         int port) {
    auto action = [this, cardId, version](QH::AbstractNodeInfo *node) {

        auto dist = static_cast<NodeInfo*>(node);

        auto api = getApi(node);
        if (api) {
            api->sendUpdateCard(cardId, version, dist);
        }
    };

    if (domain.isEmpty()) {
        return addNode(getServerHost(), port, action,
                       QH::NodeCoonectionStatus::Confirmed);
    }

    return addNode(domain, port, action,
                   QH::NodeCoonectionStatus::Confirmed);
}

bool Seller::deleteCard(unsigned int cardId, const QString &domain, int port) {
    auto action = [this, cardId](QH::AbstractNodeInfo *node) {

        auto dist = static_cast<NodeInfo*>(node);

        auto api = getApi(node);
        if (api) {
            api->deleteCard(cardId, dist);
        }
    };

    if (domain.isEmpty()) {
        return addNode(getServerHost(), port, action,
                       QH::NodeCoonectionStatus::Confirmed);
    }

    return addNode(domain, port, action,
                   QH::NodeCoonectionStatus::Confirmed);
}

BaseNode::NodeType Seller::nodeType() const {
    return static_cast<BaseNode::NodeType>(API::APIBase::NodeType::Seller);
}

void Seller::nodeConnected(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConnected(node);
}

void Seller::nodeConfirmend(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConfirmend(node);
}
}
