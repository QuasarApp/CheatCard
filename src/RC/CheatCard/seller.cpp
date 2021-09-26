//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "seller.h"

#include <datastructures.h>
#include <session.h>
#include <user.h>
#include <userscards.h>
namespace RC {

Seller::Seller(QH::ISqlDBCache *db): BaseNode(db) {
}

bool Seller::incrementPurchases(const QSharedPointer<UsersCards> &usersCardsData, int purchasesCount) {

    if (!usersCardsData)
        return false;

    usersCardsData->setPurchasesNumber(usersCardsData->getPurchasesNumber() + purchasesCount);

    if (!db()->insertIfExistsUpdateObject(usersCardsData)) {
        return false;
    }

    emit sigPurchaseWasSuccessful(usersCardsData);

    return true;
}

bool Seller::incrementPurchase(const QSharedPointer<UserHeader> &userHeaderData,
                               unsigned int cardId, int purchasesCount,
                               const QString &domain, int port) {
    if (userHeaderData->getSessionId() <= 0)
        return false;

    auto session = QSharedPointer<Session>::create();

    session->setSessionId(userHeaderData->getSessionId());
    session->setUsercardId(UsersCards::genId(userHeaderData->getUserId(), cardId));

    if (!db()->insertObject(session)) {
        return false;
    }

    _lastRequested += userHeaderData->getSessionId();

    User userrquest;
    userrquest.setId(userHeaderData->getUserId());
    auto dbUser = db()->getObject(userrquest);

    if (!dbUser) {

        dbUser = QSharedPointer<User>::create();
        dbUser->setKey(userHeaderData->token());
        dbUser->setId(userHeaderData->getUserId());

        if (!db()->insertIfExistsUpdateObject(dbUser)) {
            QuasarAppUtils::Params::log("Failed to update data", QuasarAppUtils::Warning);

        }

    }

    auto userCardsData = getUserCardData(userHeaderData->getUserId(), cardId);
    if (!userCardsData) {
        userCardsData = QSharedPointer<UsersCards>::create();
        userCardsData->setOwner(false);
        userCardsData->setUser(userHeaderData->getUserId());
        userCardsData->setPurchasesNumber(0);
        userCardsData->setCard(cardId);
    }

    if (!incrementPurchases(userCardsData, purchasesCount)) {
        QuasarAppUtils::Params::log("Failed to update data", QuasarAppUtils::Error);

        return false;
    }


    return addNode(domain, port);
}

void Seller::nodeConfirmend(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConfirmend(node);

    for (unsigned long long sessionId: qAsConst(_lastRequested)) {
        Session session;

        session.setSessionId(sessionId);

        sendData(&session, node->networkAddress());
    }
}
}
