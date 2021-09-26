//#
//# Copyright (C) 2020-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "connectiontest.h"
#include "testdatabasewrapper.h"
#include <private/card.h>
#include <private/user.h>
#include <thread>
#include <chrono>
#include <CheatCard/visitor.h>
#include <CheatCard/seller.h>
#include <CheatCard/server.h>
#include <type_traits>

#define TEST_CHEAT_PORT 15001
#define TEST_CHEAT_HOST "localhost"


void softDeleteWrap(TestDataBaseWrapper* obj) {
    obj->softDelete();
}

template <class NodeType>
QSharedPointer<NodeType> makeNode() {
    srand(time(0) + rand());
    QString randomNodeName = QByteArray::number(rand()).toHex();

    auto sallerDb = QSharedPointer<TestDataBaseWrapper>(
                new TestDataBaseWrapper(randomNodeName),
                softDeleteWrap);

    if (std::is_some<NodeType, RC::Seller>::value) {
        sallerDb = QSharedPointer<TestDataBaseWrapper>(
                    new TestDataBaseWrapper(randomNodeName,
                                            ":/sql/units/sql/TestSallerDb.sql"),
                    softDeleteWrap);
    }

    sallerDb->initSqlDb();

    return QSharedPointer<NodeType>::create(sallerDb->db());
}

ConnectionTest::ConnectionTest() {

}

ConnectionTest::~ConnectionTest() {

}

void ConnectionTest::test() {
    QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).removeRecursively();

    firstContact();
    proxyServertests();
    //    multipleUsersConnect();
    //    longTimeWorkdTest();
}

void ConnectionTest::firstContact() {

    auto seller = makeNode<RC::Seller>();
    auto client = makeNode<RC::Visitor>();
    auto server = makeNode<RC::Server>();

    // random session
    unsigned long long session = rand() * rand();

    // run server
    QVERIFY(server->run(TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    seller.incrementPurchase();

    QVERIFY(seller->sendLastSession(session));

    // first connect should be finsished successful
    connectionNodeJonb(saller, client);

    QVERIFY(saller->finishedResult() == TestDataTransfer::FinishedSuccessful);
    QVERIFY(client->finishedResult() == TestDataTransfer::FinishedSuccessful);

    QVERIFY(saller->getPurchasesCount(client->activeUser()->userId(),
                                      saller->activeCard()->cardId()) == 1);

    QVERIFY(client->getPurchasesCount(client->activeUser()->userId(),
                                      saller->activeCard()->cardId()) == 1);


    // second connect should be failed because second connect is very fast

    connectionNodeJonb(saller, client);

    QVERIFY(saller->finishedResult() == TestDataTransfer::WrongPackage);
    QVERIFY(client->finishedResult() == TestDataTransfer::ConnectionLost);

    QVERIFY(saller->getPurchasesCount(client->activeUser()->userId(),
                                      saller->activeCard()->cardId()) == 1);

    QVERIFY(client->getPurchasesCount(client->activeUser()->userId(),
                                      saller->activeCard()->cardId()) == 1);

    // waiting for availabe next connection
    std::this_thread::sleep_for(std::chrono::seconds(10));


    connectionNodeJonb(saller, client);

    QVERIFY(saller->finishedResult() == TestDataTransfer::FinishedSuccessful);
    QVERIFY(client->finishedResult() == TestDataTransfer::FinishedSuccessful);

    QVERIFY(saller->getPurchasesCount(client->activeUser()->userId(),
                                      saller->activeCard()->cardId()) == 2);

    QVERIFY(client->getPurchasesCount(client->activeUser()->userId(),
                                      saller->activeCard()->cardId()) == 2);


}

void ConnectionTest::proxyServertests() {
    //    auto saller = makeNode(TestDataTransfer::Saller);
    //    auto client = makeNode(TestDataTransfer::Client);
    //    auto proxy = makeNode(TestDataTransfer::ProxyServer);

    //    auto connectionNodeJonb = [this](QSharedPointer<TestDataTransfer> NodeA,
    //                                QSharedPointer<TestDataTransfer> NodeB){
    //        connectNodes(NodeA, NodeB);

    //        QVERIFY(wait([NodeA, NodeB]() {
    //            return NodeA->isFinished() && NodeB->isFinished();
    //        }, RC_WAIT_TIME + 1000));
    //    };

    //    // first connect should be finsished successful
    //    connectionNodeJonb(saller, proxy);

    //    QVERIFY(saller->finishedResult() == TestDataTransfer::FinishedSuccessful);
    //    QVERIFY(proxy->finishedResult() == TestDataTransfer::FinishedSuccessful);

    //    QVERIFY(saller->getPurchasesCount(client->activeUser()->userId(),
    //                              saller->activeCard()->cardId()) == 1);

    //    QVERIFY(proxy->getPurchasesCount(client->activeUser()->userId(),
    //                              saller->activeCard()->cardId()) == 1);


    //    // first connect should be finsished successful
    //    connectionNodeJonb(proxy, client);

    //    QVERIFY(proxy->finishedResult() == TestDataTransfer::FinishedSuccessful);
    //    QVERIFY(client->finishedResult() == TestDataTransfer::FinishedSuccessful);

    //    QVERIFY(proxy->getPurchasesCount(client->activeUser()->userId(),
    //                              saller->activeCard()->cardId()) == 1);

    //    QVERIFY(client->getPurchasesCount(client->activeUser()->userId(),
    //                              saller->activeCard()->cardId()) == 1);

}

void ConnectionTest::multipleUsersConnect() {
    QVERIFY(false);
}

void ConnectionTest::longTimeWorkdTest() {
    QVERIFY(false);
}

//void ConnectionTest::connectNodes(const QSharedPointer<TestDataTransfer> &nodeA,
//                                  const QSharedPointer<TestDataTransfer> &nodeB) {

//    TestDataTransferSocket *socket1 = new TestDataTransferSocket(nullptr);
//    TestDataTransferSocket *socket2 = new TestDataTransferSocket(socket1);
//    socket1->setAnother(socket2);

//    nodeA->addTestConnection(socket1);
//    nodeB->addTestConnection(socket2);

//}
