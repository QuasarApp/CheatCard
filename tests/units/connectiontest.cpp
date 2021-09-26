//#
//# Copyright (C) 2020-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "connectiontest.h"
#include "testdatabasewrapper.h"
#include "testserver.h"
#include <private/card.h>
#include <private/datastructures.h>
#include <private/user.h>
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
    QString randomNodeName = QByteArray::number(rand()).toHex();

    auto sallerDb = QSharedPointer<TestDataBaseWrapper>(
                new TestDataBaseWrapper(randomNodeName),
                softDeleteWrapDB);

    if (std::is_same<NodeType, RC::Seller>::value ||
            std::is_base_of<NodeType, RC::Seller>::value ) {

        sallerDb = QSharedPointer<TestDataBaseWrapper>(
                    new TestDataBaseWrapper(randomNodeName,
                                            ":/sql/units/sql/TestSallerDb.sql"),
                    softDeleteWrapDB);
    }

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
    unsigned long long session = rand() * rand();

    // run server
    QVERIFY(server->run(TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    auto user = makeUser();
    auto obj = QSharedPointer<RC::UserHeader>::create();

    obj->setSessionId(session);
    obj->setToken(user->getKey());
    obj->setUserId(user->userId());

    // 3619648333 This is card id from test database.
    unsigned int cardId = 3619648333;
    QVERIFY(seller->incrementPurchase(obj, cardId, 1, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    QVERIFY(wait([server](){
        return server->isDataReceivedSuccessful();
    }, 1000));


    QVERIFY(server->confirmendCount() == 0);
    QVERIFY(server->getPurchaseCount(user->userId(), cardId) == 1);

}

QSharedPointer<RC::User> ConnectionTest::makeUser() const {
    auto result = QSharedPointer<RC::User>::create();

    result->setName(QString("User%0").arg(rand()));

    return result;
}
