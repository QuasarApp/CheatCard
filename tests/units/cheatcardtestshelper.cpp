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

NetworkResult CheatCardTestsHelper::deployNetwork(QString host, int port,
                                                  unsigned int clientsCount,
                                                  bool connectToServer) {
    NetworkResult result;
    result.server = makeNode<TestServer>();

    if (!result.server->run(host, port))
        return {};

    for (unsigned int i = 0; i < clientsCount; ++i) {
        auto node = makeNode<TestClient>();
        result.clients.insert(node->currntUserKey(), node);

        if (connectToServer) {
            node->connectToServer(host, port);

            if (!TestUtils::wait([node]() {
                    return node->isSynced();
                }, WAIT_TIME)) {
                return {};
            }
        }
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

    if (!owner->cardWasUpdated(card->cardId())) {
        return nullptr;
    }

    if (!owner->db()->saveCard(card)) {
        return nullptr;
    }

    // wait responce from the server about adding new card.
    TestUtils::wait([card, owner]() {
        return bool(owner->getUserData(card->ownerSignature(), card->cardId()));
    }, WAIT_TIME);

    return card;

}

void CheatCardTestsHelper::makeSeals(const QSharedPointer<TestClient> &seller,
                                     const QSharedPointer<TestClient> &client,
                                     const QByteArray &cardId,
                                     unsigned int sealCount) {

    auto clientKey = client->currntUserKey();
    for ( unsigned int i = 0; i < sealCount; ++i ) {
        unsigned int count = seller->getPurchaseCount(client->currntUserKey(), cardId);

        QVERIFY(seller->incrementPurchase(clientKey, cardId));

        QVERIFY(TestUtils::wait([seller, clientKey, cardId, count]() {
            unsigned int currentCount = seller->getPurchaseCount(clientKey, cardId);
            return currentCount == count + 1;
        }, WAIT_TIME));

    }

#ifdef QT_DEBUG
    qInfo() << "cheack mooving card ";
#endif

    QVERIFY(TestUtils::wait([client, cardId, clientKey, sealCount]() {
        auto card = client->getCard(cardId);
        unsigned int currentCount = client->getPurchaseCount(clientKey, cardId);

        return card && card->isValid() && currentCount == sealCount;
    }, WAIT_TIME));

#ifdef QT_DEBUG
    qInfo() << "makeSeals finished successfull ";
#endif

}

void CheatCardTestsHelper::makeSealsFor(const QSharedPointer<TestClient> &seller,
                                        const QByteArray &client,
                                        const QByteArray &cardId,
                                        unsigned int sealCount) {

    for ( unsigned int i = 0; i < sealCount; ++i ) {
        unsigned int count = seller->getPurchaseCount(client, cardId);

        QVERIFY(seller->incrementPurchase(client, cardId));
        QVERIFY(TestUtils::wait([client, seller, cardId, count]() {
            unsigned int currentCount = seller->getPurchaseCount(client, cardId);
            return currentCount == count + 1;
        }, WAIT_TIME));

    }

    QVERIFY(TestUtils::wait([client, cardId, seller, sealCount]() {
        unsigned int currentCount = seller->getPurchaseCount(client, cardId);
        return currentCount == sealCount;
    }, WAIT_TIME));

#ifdef QT_DEBUG
    qInfo() << "makeSeals finished successfull ";
#endif

}


void CheatCardTestsHelper::checkAccess(const QSharedPointer<TestClient> &seller,
                                       const QSharedPointer<TestServer> &server,
                                       const QByteArray &client,
                                       const QByteArray &cardId,
                                       bool shouldBe) {

    unsigned int count = seller->getPurchaseCount(client, cardId);
    unsigned int serverCount = server->getPurchaseCount(client, cardId);

    // do not check access of the not synced nodes.
    QVERIFY(count == serverCount);

    auto cbTrue = [client, seller, cardId, count]() {
        unsigned int currentCount = seller->getPurchaseCount(client, cardId);
        return currentCount == count + 1 && !seller->getLastErrrorCode();
    };

    auto cbFalse = [client, seller, cardId, count]() {
        unsigned int currentCount = seller->getPurchaseCount(client, cardId);
        return currentCount == count && seller->getLastErrrorCode();
    };

    seller->resetLastErrors();

    QVERIFY(seller->incrementPurchase(client, cardId));
    QVERIFY(TestUtils::wait([shouldBe, cbTrue, cbFalse](){return (shouldBe)? cbTrue() : cbFalse();}, WAIT_TIME));
}

const QSharedPointer<RC::Interfaces::iDB> &CheatCardTestsHelper::objectFactory() {
    static auto db = RC::DB::makeDb(1);
    return db;
}

void softDeleteWrapNode(RC::BaseNode *obj) {
    obj->softDelete();
}
