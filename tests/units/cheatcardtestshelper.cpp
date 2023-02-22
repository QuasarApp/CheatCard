//#
//# Copyright (C) 2020-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "cheatcardtestshelper.h"
#include "qtestcase.h"
#include "testserver.h"
#include "testclient.h"
#include "testutils.h"

CheatCardTestsHelper::CheatCardTestsHelper() {

}

NetworkResult CheatCardTestsHelper::deployNetwork(QString host, int port, unsigned int clientsCount) {
    NetworkResult result;
    result.server = makeNode<TestServer>();

    if (!result.server->run(host, port))
        return {};

    for (unsigned int i = 0; i < clientsCount; ++i) {
        auto node = makeNode<TestClient>();
        result.clients.insert(node->currntUserKey(), node);
        node->connectToServer(host, port);
    }

    return result;
}

QSharedPointer<RC::Interfaces::iUser> CheatCardTestsHelper::makeUser() {
    auto newUser = objectFactory()->makeEmptyUser();

    newUser->setName(QString("User%0").arg(rand()));
    return newUser;
}

QSharedPointer<RC::Interfaces::iContacts> CheatCardTestsHelper::makeContact() {
    return objectFactory()->makeEmptyContact();
}

QSharedPointer<RC::Interfaces::iCard>
CheatCardTestsHelper::makeCard(const QSharedPointer<TestClient> &owner, unsigned int freeItemCount) {
    auto card = objectFactory()->makeEmptyCard();
    card->setOwnerSignature(owner->currntUserKey());
    card->setFreeItemName("bonus");
    card->setFreeIndex(freeItemCount);

    if (!owner->db()->saveCard(card)) {
        return nullptr;
    }

    return card;

}

void CheatCardTestsHelper::makeSeals(const QSharedPointer<TestClient> &seller,
                                     const QSharedPointer<TestClient> &client,
                                     const QByteArray &cardId,
                                     unsigned int sealCount) {

    auto clientKey = client->currntUserKey();
    for ( unsigned int i = 0; i < sealCount; ++i ) {
        unsigned int count = seller->getPurchaseCount(client->currntUserKey(), cardId);

        seller->incrementPurchase(clientKey, cardId);
        QVERIFY(TestUtils::wait([client, clientKey, cardId, count]() {
            unsigned int currentCount = client->getPurchaseCount(clientKey, cardId);
            return currentCount == count + 1;
        }, WAIT_TIME));

    }

    QVERIFY(TestUtils::wait([client, cardId]() {
        auto card = client->getCard(cardId);
        return card && card->isValid();
    }, WAIT_TIME));
}

const QSharedPointer<RC::Interfaces::iDB> &CheatCardTestsHelper::objectFactory() {
    static auto db = RC::DB::makeDb(1);
    return db;
}

void softDeleteWrapNode(RC::BaseNode *obj) {
    obj->softDelete();
}
