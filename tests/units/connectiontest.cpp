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
//    userTrySendWrongData();
//    multipleUsersConnect();
//    longTimeWorkdTest();
}

void ConnectionTest::firstContact() {

    auto saller = makeNode(TestDataTransfer::Saller);
    auto client = makeNode(TestDataTransfer::Client);

    connectNodes(saller, client);

    QVERIFY(wait([saller, client]() {
        if (saller->isFinished()) {
            if (saller->finishedResult() != TestDataTransfer::FinishedSuccessful)
                return true;
        }

        if (client->isFinished()) {
            if (client->finishedResult() != TestDataTransfer::FinishedSuccessful)
                return true;
        }

        return saller->isFinished() && client->isFinished();
    }, RC_WAIT_TIME + 1000));

    QVERIFY(saller->finishedResult() == TestDataTransfer::FinishedSuccessful);
    QVERIFY(client->finishedResult() == TestDataTransfer::FinishedSuccessful);

    QVERIFY(saller->getPurchasesCount(client->activeUser()->userId(),
                              saller->activeCard()->cardId()) == 1);

    QVERIFY(client->getPurchasesCount(client->activeUser()->userId(),
                              saller->activeCard()->cardId()) == 1);

}

void ConnectionTest::userTrySendWrongData() {
    QVERIFY(false);
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
        card.setId(QVariant::fromValue(3618836081));
        auto activeCard = sallerDb->db()->getObject(card);

        result->setActiveCard(activeCard);
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
