//#
//# Copyright (C) 2020-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "CheatCard/clearolddata.h"
#include "CheatCard/userheader.h"
#include "api.h"
#include "cheatcardtestshelper.h"
#include "connectiontest.h"
#include "testserver.h"

#include <thread>
#include <chrono>
#include <testseller.h>
#include <testvisitor.h>
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

void ConnectionTest::addSeal(const QSharedPointer<TestSeller> &seller,
                             const QSharedPointer<TestVisitor> &client,
                             const QSharedPointer<TestServer> &server,
                             const QSharedPointer<RC::Interfaces::iUser> &user,
                             unsigned int cardId, int sealsCount,
                             QSharedPointer<RC::UserHeader> & resultUserHeader,
                             const QString &host, int port)
{

    // random session
    long long session = rand() * rand();

    unsigned int userId = user->id();

    resultUserHeader->setSessionId(session);
    resultUserHeader->setToken(user->getKey());
    resultUserHeader->setUserId(userId);

    for (int i = 0; i < sealsCount; i++) {

        qDebug () << "Add seal " << i << "/" << sealsCount;

        QVERIFY(seller->incrementPurchase(resultUserHeader, cardId, 1, host, port));

        QVERIFY(wait([server, cardId](){
            auto card = server->getCard(cardId);
            return card && card->isValid();
        }, WAIT_TIME));

        QVERIFY(wait([server, userId, cardId, i](){
            return server->getPurchaseCount(userId, cardId) == (i + 1);
        }, WAIT_TIME));

        QVERIFY(wait([server](){
            return server->connectionsCount() == 0;
        }, WAIT_TIME));

        QVERIFY(client->checkCardData(session, host, port));

        QVERIFY(wait([client, cardId](){
            auto card = client->getCard(cardId);
            return card && card->isValid();
        }, WAIT_TIME));


        QVERIFY(wait([client, userId, cardId, i](){
            return client->getPurchaseCount(userId, cardId) == (i + 1);
        }, WAIT_TIME));

        QVERIFY(wait([server](){
            return server->connectionsCount() == 0;
        }, WAIT_TIME));


        QVERIFY(seller->getPurchaseCount(userId, cardId) == (i + 1));

    }
}

void ConnectionTest::firstContact() {


    QSharedPointer<TestSeller> seller;
    QSharedPointer<TestVisitor> client;
    QSharedPointer<TestServer> server;

    qDebug() << "TEST API V2";

    seller = CheatCardTestsHelper::makeNode<TestSeller>(":/sql/units/sql/TestSallerDb.sql");
    client = CheatCardTestsHelper::makeNode<TestVisitor>();
    server = CheatCardTestsHelper::makeNode<TestServer>();
    seller->setCurrentUser(seller->getUser(CheatCardTestsHelper::testUserId()));

    RC::API::init({2}, seller->getDBObject(), seller.data());
    RC::API::init({2}, client->getDBObject(), client.data());
    RC::API::init({2}, server->getDBObject(), server.data());

    apiTest(seller, client, server);

}

void ConnectionTest::apiTest(const QSharedPointer<TestSeller> &seller,
                             const QSharedPointer<TestVisitor> &client,
                             const QSharedPointer<TestServer> &server) {

    // run server
    QVERIFY(server->run(TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    auto user = CheatCardTestsHelper::makeUser();
    auto obj = QSharedPointer<RC::UserHeader>::create();



    // 3619648333 This is card id from test database.
    unsigned int cardId = CheatCardTestsHelper::testCardId();
    unsigned int userId = user->id();

    addSeal(seller, client, server, user, cardId, 100, obj, TEST_CHEAT_HOST,  TEST_CHEAT_PORT);

    int sellerFreeItems = seller->getFreeItemsCount(user->id(), cardId);
    int visitorFreeItems = client->getFreeItemsCount(user->id(), cardId);

    QVERIFY(sellerFreeItems == visitorFreeItems);
    QVERIFY(sellerFreeItems == 16);

    // 3619648333 This is card id from test database.

    QVERIFY(seller->sentDataToServerReceive(obj, cardId, 16, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    QVERIFY(wait([server, cardId, userId]() {
        return server->getFreeItemsCount(userId, cardId) == 0;
    }, WAIT_TIME));

    QVERIFY(client->checkCardData(obj->getSessionId(), TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    QVERIFY(wait([client, cardId, userId]() {
        return client->getFreeItemsCount(userId, cardId) == 0;
    }, WAIT_TIME));


    QVERIFY(seller->getFreeItemsCount(user->id(), cardId) == 0);


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

