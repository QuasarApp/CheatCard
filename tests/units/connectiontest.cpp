//#
//# Copyright (C) 2020-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "CheatCard/clearolddata.h"
#include "api.h"
#include "cheatcardtestshelper.h"
#include "connectiontest.h"
#include "testserver.h"

#include <thread>
#include <chrono>
#include <testclient.h>
#include <type_traits>

#include <rci/objects/iuser.h>

#define TEST_CHEAT_PORT 15001
#define TEST_CHEAT_HOST "localhost"


ConnectionTest::ConnectionTest() {

}

ConnectionTest::~ConnectionTest() {

}

void ConnectionTest::test() {
    QDir(DEFAULT_DB_PATH).removeRecursively();
    firstContact();
}

void ConnectionTest::addSeal(const QSharedPointer<TestClient> &seller,
                             const QSharedPointer<TestClient> &client,
                             const QSharedPointer<TestServer> &server,
                             const QByteArray& userKey,
                             const QByteArray& cardId,
                             int sealsCount)
{
    for (int i = 0; i < sealsCount; i++) {

        qDebug () << "Add seal " << i << "/" << sealsCount;

        QVERIFY(seller->incrementPurchase(userKey, cardId, 1));

        QVERIFY(wait([server, cardId](){
            auto card = server->getCard(cardId);
            return card && card->isValid();
        }, WAIT_TIME));

        QVERIFY(wait([server, userKey, cardId, i](){
            return server->getPurchaseCount(userKey, cardId) == (i + 1);
        }, WAIT_TIME));

        QVERIFY(wait([server](){
            return server->connectionsCount() == 0;
        }, WAIT_TIME));

        QVERIFY(wait([client, cardId](){
            auto card = client->getCard(cardId);
            return card && card->isValid();
        }, WAIT_TIME));


        QVERIFY(wait([client, userKey, cardId, i](){
            return client->getPurchaseCount(userKey, cardId) == (i + 1);
        }, WAIT_TIME));

        QVERIFY(wait([server](){
            return server->connectionsCount() == 0;
        }, WAIT_TIME));


        QVERIFY(seller->getPurchaseCount(userKey, cardId) == (i + 1));

    }
}

void ConnectionTest::firstContact() {


    QSharedPointer<TestClient> seller;
    QSharedPointer<TestClient> client;
    QSharedPointer<TestServer> server;

    qDebug() << "TEST API V2";

    seller = CheatCardTestsHelper::makeNode<TestClient>(":/sql/units/sql/TestSallerDb.sql");
    client = CheatCardTestsHelper::makeNode<TestClient>();
    server = CheatCardTestsHelper::makeNode<TestServer>();
    seller->setCurrntUserKey(CheatCardTestsHelper::testUserPublicKey());

    RC::API::init({2}, seller->getDBObject(), seller.data());
    RC::API::init({2}, client->getDBObject(), client.data());
    RC::API::init({2}, server->getDBObject(), server.data());

    apiTest(seller, client, server);

}

void ConnectionTest::apiTest(const QSharedPointer<TestClient> &seller,
                             const QSharedPointer<TestClient> &client,
                             const QSharedPointer<TestServer> &server) {

    // run server
    QVERIFY(server->run(TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    auto user = CheatCardTestsHelper::makeUser();

    QByteArray cardId = CheatCardTestsHelper::testCardId();
    QByteArray userId = user->getKey();

    QVERIFY(client->connectToServer(TEST_CHEAT_HOST, TEST_CHEAT_PORT));
    QVERIFY(seller->connectToServer(TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    client->setCurrntUserKey(user->getKey());

    addSeal(seller, client, server, userId, cardId, 100);

    int sellerFreeItems = seller->getFreeItemsCount(userId, cardId);
    int visitorFreeItems = client->getFreeItemsCount(userId, cardId);

    QVERIFY(sellerFreeItems == visitorFreeItems);
    QVERIFY(sellerFreeItems == 16);

    QVERIFY(seller->incrementReceived(userId, cardId, 16));

    QVERIFY(wait([server, cardId, userId]() {
        return server->getFreeItemsCount(userId, cardId) == 0;
    }, WAIT_TIME));

    QVERIFY(wait([client, cardId, userId]() {
        return client->getFreeItemsCount(userId, cardId) == 0;
    }, WAIT_TIME));


    QVERIFY(seller->getFreeItemsCount(userId, cardId) == 0);


    // check rad on serve before clear
    QVERIFY(wait([server, cardId](){
        auto card = server->getCard(cardId);
        return card && card->isValid();
    }, WAIT_TIME));

    auto task = QSharedPointer<RC::ClearOldData>::create(server->getDBObject());
    task->setTime(0);
    task->setMode(QH::ScheduleMode::SingleWork);
    QVERIFY(server->sheduleTask(task));

    // check card after general clear. card should be leave
    QVERIFY(wait([server, cardId](){
        auto card = server->getCard(cardId);
        return card && card->isValid();
    }, WAIT_TIME));

    // run force clear
    task = QSharedPointer<RC::ClearOldData>::create(server->getDBObject(), -1);
    task->setTime(0);
    task->setMode(QH::ScheduleMode::SingleWork);
    QVERIFY(server->sheduleTask(task));

    // check card after clear. card should be removed
    QVERIFY(wait([server, cardId](){
        auto card = server->getCard(cardId);
        return !card;
    }, WAIT_TIME));

    // To Do check free items count
}

