//#
//# Copyright (C) 2020-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "connectiontest.h"
#include "testdatabasewrapper.h"
#include "testdatatransfer.h"
#include <private/card.h>
#include <private/user.h>
#include <thread>
#include <chrono>

void softDeleteWrap(TestDataBaseWrapper* obj) {
    obj->softDelete();
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

    auto saller = makeNode(TestDataTransfer::Saller);
    auto client = makeNode(TestDataTransfer::Client);

    auto connectionNodeJonb = [this](QSharedPointer<TestDataTransfer> saller,
                                QSharedPointer<TestDataTransfer> client){
        connectNodes(saller, client);

        QVERIFY(wait([saller, client]() {
            return saller->isFinished() && client->isFinished();
        }, RC_WAIT_TIME + 1000));


    };

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
    auto saller = makeNode(TestDataTransfer::Saller);
    auto client = makeNode(TestDataTransfer::Client);
    auto proxy = makeNode(TestDataTransfer::ProxyServer);

    auto connectionNodeJonb = [this](QSharedPointer<TestDataTransfer> NodeA,
                                QSharedPointer<TestDataTransfer> NodeB){
        connectNodes(NodeA, NodeB);

        QVERIFY(wait([NodeA, NodeB]() {
            return NodeA->isFinished() && NodeB->isFinished();
        }, RC_WAIT_TIME + 1000));


    };

    // first connect should be finsished successful
    connectionNodeJonb(saller, proxy);

    QVERIFY(saller->finishedResult() == TestDataTransfer::FinishedSuccessful);
    QVERIFY(proxy->finishedResult() == TestDataTransfer::FinishedSuccessful);

    QVERIFY(saller->getPurchasesCount(client->activeUser()->userId(),
                              saller->activeCard()->cardId()) == 1);

    QVERIFY(proxy->getPurchasesCount(client->activeUser()->userId(),
                              saller->activeCard()->cardId()) == 1);


    // first connect should be finsished successful
    connectionNodeJonb(proxy, client);

    QVERIFY(proxy->finishedResult() == TestDataTransfer::FinishedSuccessful);
    QVERIFY(client->finishedResult() == TestDataTransfer::FinishedSuccessful);

    QVERIFY(proxy->getPurchasesCount(client->activeUser()->userId(),
                              saller->activeCard()->cardId()) == 1);

    QVERIFY(client->getPurchasesCount(client->activeUser()->userId(),
                              saller->activeCard()->cardId()) == 1);

}

void ConnectionTest::multipleUsersConnect() {
    QVERIFY(false);
}

void ConnectionTest::longTimeWorkdTest() {
    QVERIFY(false);
}

QSharedPointer<TestDataTransfer>
ConnectionTest::makeNode(RC::IConnectorBackEnd::Mode mode) {

    srand(time(0) + rand());
    QString randomNodeName = QByteArray::number(rand()).toHex();
    auto sallerDb = QSharedPointer<TestDataBaseWrapper>(
                new TestDataBaseWrapper(randomNodeName, mode), softDeleteWrap);


    sallerDb->initSqlDb();
    auto result = QSharedPointer<TestDataTransfer>::create(
                mode, sallerDb);


    if (mode == RC::IConnectorBackEnd::Saller) {

        RC::Card card;
        card.setId(QVariant::fromValue(2887308033));
        auto activeCard = sallerDb->db()->getObject(card);

        result->setActiveCard(activeCard, 1);
    } else {
        result->setActiveUser(QSharedPointer<RC::User>::create());
    }

    return result;
}

void ConnectionTest::connectNodes(const QSharedPointer<TestDataTransfer> &nodeA,
                                  const QSharedPointer<TestDataTransfer> &nodeB) {

    TestDataTransferSocket *socket1 = new TestDataTransferSocket(nullptr);
    TestDataTransferSocket *socket2 = new TestDataTransferSocket(socket1);
    socket1->setAnother(socket2);

    nodeA->addTestConnection(socket1);
    nodeB->addTestConnection(socket2);

}
