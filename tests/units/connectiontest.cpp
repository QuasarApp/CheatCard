//#
//# Copyright (C) 2020-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cheatcardtestshelper.h"
#include "connectiontest.h"
#include "testdatabasewrapper.h"
#include "testserver.h"

#include <CheatCard/api/api0/card.h>
#include <CheatCard/api/api0/user.h>
#include <CheatCard/api/api0/userheader.h>

#include <thread>
#include <chrono>
#include <CheatCard/visitor.h>
#include <CheatCard/seller.h>
#include <CheatCard/server.h>
#include <testseller.h>
#include <testvisitor.h>
#include <type_traits>
#include <CheatCard/api/apiv1.h>
#include <CheatCard/api/apiv1-5.h>

#define TEST_CHEAT_PORT 15001
#define TEST_CHEAT_HOST "localhost"


ConnectionTest::ConnectionTest() {

}

ConnectionTest::~ConnectionTest() {

}

void ConnectionTest::test() {
    QDir(APP_DATA_LOCATION).removeRecursively();
    firstContact();
}

void ConnectionTest::firstContact() {


    auto seller = CheatCardTestsHelper::makeNode<TestSeller>();
    auto client = CheatCardTestsHelper::makeNode<TestVisitor>();
    auto server = CheatCardTestsHelper::makeNode<TestServer>();


    qDebug() << "TEST API V1";

    seller = CheatCardTestsHelper::makeNode<TestSeller>();
    client = CheatCardTestsHelper::makeNode<TestVisitor>();
    server = CheatCardTestsHelper::makeNode<TestServer>();
    seller->setCurrentUser(seller->getUser(CheatCardTestsHelper::testUserId()));

    seller->addApiParser<RC::ApiV1>();
    client->addApiParser<RC::ApiV1>();
    server->addApiParser<RC::ApiV1>();
    server->addApiParser<RC::ApiV1_5>();

    apiTest(seller, client, server);

    qDebug() << "TEST API V1(seller)";

    seller = CheatCardTestsHelper::makeNode<TestSeller>();
    client = CheatCardTestsHelper::makeNode<TestVisitor>();
    server = CheatCardTestsHelper::makeNode<TestServer>();
    seller->setCurrentUser(seller->getUser(CheatCardTestsHelper::testUserId()));

    seller->addApiParser<RC::ApiV1>();
    client->addApiParser<RC::ApiV1_5>();
    server->addApiParser<RC::ApiV1>();
    server->addApiParser<RC::ApiV1_5>();

    apiTest(seller, client, server);

    qDebug() << "TEST API V1(client)";

    seller = CheatCardTestsHelper::makeNode<TestSeller>();
    client = CheatCardTestsHelper::makeNode<TestVisitor>();
    server = CheatCardTestsHelper::makeNode<TestServer>();
    seller->setCurrentUser(seller->getUser(CheatCardTestsHelper::testUserId()));

    seller->addApiParser<RC::ApiV1_5>();
    client->addApiParser<RC::ApiV1>();
    server->addApiParser<RC::ApiV1>();
    server->addApiParser<RC::ApiV1_5>();

    apiTest(seller, client, server);


    qDebug() << "TEST API V1-5";

    seller = CheatCardTestsHelper::makeNode<TestSeller>();
    client = CheatCardTestsHelper::makeNode<TestVisitor>();
    server = CheatCardTestsHelper::makeNode<TestServer>();
    seller->setCurrentUser(seller->getUser(CheatCardTestsHelper::testUserId()));

    seller->addApiParser<RC::ApiV1_5>();
    client->addApiParser<RC::ApiV1_5>();
    server->addApiParser<RC::ApiV1>();
    server->addApiParser<RC::ApiV1_5>();

    apiTest(seller, client, server);

}

void ConnectionTest::apiTest(const QSharedPointer<TestSeller> &seller,
                             const QSharedPointer<TestVisitor> &client,
                             const QSharedPointer<TestServer> &server) {

    // random session
    long long session = rand() * rand();

    // run server
    QVERIFY(server->run(TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    auto user = CheatCardTestsHelper::makeUser();
    auto obj = QSharedPointer<RC::API::UserHeader>::create();

    obj->setSessionId(session);
    obj->setToken(user->getKey());
    obj->setUserId(user->userId());

    // 3619648333 This is card id from test database.
    unsigned int cardId = CheatCardTestsHelper::testCardId();
    for (int i = 0; i < 100; i++) {

        qDebug () << "test case " << i << "/" << 100;

        QVERIFY(seller->incrementPurchase(obj, cardId, 1, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

        QVERIFY(wait([server, cardId](){
            auto card = server->getCard(cardId);
            return card && card->isValid();
        }, WAIT_TIME));


        QVERIFY(wait([server, user, cardId, i](){
            return server->getPurchaseCount(user->userId(), cardId) == (i + 1);
        }, WAIT_TIME));

        QVERIFY(wait([server](){
            return server->connectionsCount() == 0;
        }, WAIT_TIME));

        QVERIFY(client->checkCardData(session, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

        QVERIFY(wait([client, cardId](){
            auto card = client->getCard(cardId);
            return card && card->isValid();
        }, WAIT_TIME));


        QVERIFY(wait([client, user, cardId, i](){
            return client->getPurchaseCount(user->userId(), cardId) == (i + 1);
        }, WAIT_TIME));

        QVERIFY(wait([server](){
            return server->connectionsCount() == 0;
        }, WAIT_TIME));

    }

    int sellerFreeItems = seller->getFreeItemsCount(user->userId(), cardId);
    int visitorFreeItems = client->getFreeItemsCount(user->userId(), cardId);

    QVERIFY(sellerFreeItems == visitorFreeItems);
    QVERIFY(sellerFreeItems == 16);


    // check rad on serve before clear
    QVERIFY(wait([server, cardId](){
        auto card = server->getCard(cardId);
        return card && card->isValid();
    }, WAIT_TIME));

    auto task = QSharedPointer<RC::ClearOldData>::create();
    task->setTime(0);
    task->setMode(QH::ScheduleMode::SingleWork);
    QVERIFY(server->sheduleTask(task));

    // check card after general clear. card should be leave
    QVERIFY(wait([server, cardId](){
        auto card = server->getCard(cardId);
        return card && card->isValid();
    }, WAIT_TIME));

    // run force clear
    task = QSharedPointer<RC::ClearOldData>::create(-1);
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

