//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "CheatCard/api/api0/carddatarequest.h"
#include "CheatCard/api/api0/cardstatusrequest.h"
#include "CheatCard/dataconvertor.h"
#include "basenode.h"
#include "usersnames.h"
#include "seller.h"
#include "CheatCard/api/api0/userheader.h"
#include <CheatCard/api/api0/session.h>
#include <CheatCard/api/api0/user.h>
#include <CheatCard/api/api0/userscards.h>
#include <CheatCard/api/api1/changeuserscards.h>
#include <CheatCard/api/apiv0.h>

namespace RC {

Seller::Seller(QH::ISqlDBCache *db): BaseNode(db) {
    registerPackageType<API::CardStatusRequest>();
    registerPackageType<API::CardDataRequest>();
    registerPackageType<APIv1::ChangeUsersCards>();

}

bool Seller::incrementPurchases(const QSharedPointer<API::UsersCards> &usersCardsData,
                                int purchasesCount) {

    if (!usersCardsData)
        return false;

    if (purchasesCount < 0)
        return false;

    usersCardsData->setPurchasesNumber(usersCardsData->getPurchasesNumber() + purchasesCount);

    if (!db()->insertIfExistsUpdateObject(usersCardsData)) {
        return false;
    }

    emit sigPurchaseWasSuccessful(usersCardsData, true);

    return true;
}

QSharedPointer<API::UsersCards>
Seller::prepareData(const QSharedPointer<API::UserHeader> &userHeaderData,
                    unsigned int cardId) {

    if (!userHeaderData->isValid())
        return nullptr;

    auto session = QSharedPointer<API::Session>::create();

    session->setSessionId(userHeaderData->getSessionId());
    session->setUsercardId(API::UsersCards::genId(userHeaderData->getUserId(), cardId));

    if (!session->isValid()) {
        return nullptr;
    }

    session->setPrintError(false);
    db()->insertObject(session);

    _lastRequested[session->getSessionId()] = session;

    API::User userrquest;
    userrquest.setId(userHeaderData->getUserId());

    auto dbUser = DataConvertor::toUser(userHeaderData);

    if (dbUser->name().isEmpty()) {
        dbUser->setName(UsersNames::randomUserName());
    }

    if (!db()->insertIfExistsUpdateObject(dbUser)) {
        QuasarAppUtils::Params::log("Failed to update user data", QuasarAppUtils::Warning);

    }

    auto userCardsData = getUserCardData(userHeaderData->getUserId(), cardId);
    if (!userCardsData) {
        userCardsData = QSharedPointer<API::UsersCards>::create();
        userCardsData->setUser(userHeaderData->getUserId());
        userCardsData->setPurchasesNumber(0);
        userCardsData->setCard(cardId);
    }

    return userCardsData;
}

bool Seller::sendDataPrivate(const QString &domain, int port) {
    auto action = [this](QH::AbstractNodeInfo *node) {
        auto api = getSelectedApiParser(node).staticCast<ApiV0>();
        if (api) {
            api->sendSessions(_lastRequested, node);
            _lastRequested.clear();
        }
    };

    if (domain.isEmpty()) {
        return addNode(getServerHost(), port, action,
                       QH::NodeCoonectionStatus::Confirmed);
    }

    return addNode(domain, port, action,
                   QH::NodeCoonectionStatus::Confirmed);
}

bool Seller::incrementPurchase(const QSharedPointer<API::UserHeader> &userHeaderData,
                               unsigned int cardId, int purchasesCount,
                               const QString &domain, int port) {

    auto changes = QSharedPointer<APIv1::ChangeUsersCards>::create();


    changes->setUsercardId(API::UsersCards::genId(userHeaderData->getUserId(), cardId));
    changes->setSessionId(userHeaderData->getSessionId());
    changes->setPurchase(purchasesCount);
    changes->setSecret(currentUser()->secret());
    _lastRequested[changes->getSessionId()] = changes;

    return sendDataPrivate(domain, port);
}

bool Seller::sentDataToServerReceive(const QSharedPointer<API::UserHeader> &userHeaderData,
                                     unsigned int cardId,
                                     int receiveCount,
                                     const QString &domain,
                                     int port) {

    auto changes = QSharedPointer<APIv1::ChangeUsersCards>::create();

    changes->setUsercardId(API::UsersCards::genId(userHeaderData->getUserId(), cardId));
    changes->setSessionId(userHeaderData->getSessionId());
    changes->setReceive(receiveCount);
    changes->setSecret(currentUser()->secret());
    _lastRequested[changes->getSessionId()] = changes;

    return sendDataPrivate(domain, port);
}

NodeType Seller::nodeType() const {
    return NodeType::Seller;
}

void Seller::nodeConnected(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConnected(node);
}

void Seller::nodeConfirmend(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConfirmend(node);
}
}
