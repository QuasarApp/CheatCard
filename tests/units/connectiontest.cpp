//#
//# Copyright (C) 2020-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "connectiontest.h"
#include "testdatabasewrapper.h"
#include "testserver.h"
#include <CheatCard/card.h>
#include <CheatCard/datastructures.h>
#include <CheatCard/user.h>
#include <thread>
#include <chrono>
#include <CheatCard/visitor.h>
#include <CheatCard/seller.h>
#include <CheatCard/server.h>
#include <testseller.h>
#include <testvisitor.h>
#include <type_traits>

#define TEST_CHEAT_PORT 15001
#define TEST_CHEAT_HOST "localhost"


void softDeleteWrapDB(TestDataBaseWrapper* obj) {
    obj->softDelete();
}

void softDeleteWrapNode(RC::BaseNode* obj) {
    obj->softDelete();
}

template <class NodeType>
QSharedPointer<NodeType> makeNode() {
    srand(time(0) + rand());
    QString randomNodeName = QByteArray::number(rand()).toHex() + typeid(NodeType).name();

    TestDataBaseWrapper* source;
    if (std::is_base_of<RC::Seller,NodeType>::value ) {

        source = new TestDataBaseWrapper(randomNodeName,
                                         ":/sql/units/sql/TestSallerDb.sql");
    } else {
        source = new TestDataBaseWrapper(randomNodeName);
    }

    auto sallerDb = QSharedPointer<TestDataBaseWrapper>(source,
                                                        softDeleteWrapDB);
    sallerDb->initSqlDb();

    return QSharedPointer<NodeType>(new NodeType(sallerDb), softDeleteWrapNode);
}

ConnectionTest::ConnectionTest() {

}

ConnectionTest::~ConnectionTest() {

}

void ConnectionTest::test() {
    QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).removeRecursively();

    firstContact();
}

void ConnectionTest::firstContact() {

    auto seller = makeNode<TestSeller>();
    auto client = makeNode<TestVisitor>();
    auto server = makeNode<TestServer>();

    // random session
    long long session = rand() * rand();

    // run server
    QVERIFY(server->run(TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    auto user = makeUser();
    auto obj = QSharedPointer<RC::UserHeader>::create();

    obj->setSessionId(session);
    obj->setToken(user->getKey());
    obj->setUserId(user->userId());

    // 3619648333 This is card id from test database.
    unsigned int cardId = 3619648333;
    for (int i = 0; i < 100; i++) {

        qDebug () << "test case " << i << "/" << 100;

        QVERIFY(seller->incrementPurchase(obj, cardId, 1, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

        QVERIFY(wait([server, cardId](){
            auto card = server->getCard(cardId);
            return card && card->isValid();
        }, 1000));

        QVERIFY(wait([server](){
            return server->connectionsCount() == 0;
        }, 10000));

        QVERIFY(wait([server, user, cardId, i](){
            return server->getPurchaseCount(user->userId(), cardId) == (i + 1);
        }, 1000));


        QVERIFY(client->checkCardData(session, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

        QVERIFY(wait([client, cardId](){
            auto card = client->getCard(cardId);
            return card && card->isValid();
        }, 1000));

        QVERIFY(wait([server](){
            return server->connectionsCount() == 0;
        }, 1000));

        QVERIFY(wait([client, user, cardId, i](){
            return client->getPurchaseCount(user->userId(), cardId) == (i + 1);
        }, 1000));

    }

    int sellerFreeItems = seller->getFreeItemsCount(user->userId(), cardId);
    int visitorFreeItems = client->getFreeItemsCount(user->userId(), cardId);

    QVERIFY(sellerFreeItems == visitorFreeItems);
    QVERIFY(sellerFreeItems == 16);
    // To Do check free items count

}

QSharedPointer<RC::User> ConnectionTest::makeUser() const {
    auto result = QSharedPointer<RC::User>::create();

    result->setName(QString("User%0").arg(rand()));

    return result;
}
