//#
//# Copyright (C) 2020-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "connectiontest.h"
#include "testdatatransfer.h"

ConnectionTest::ConnectionTest() {

}

ConnectionTest::~ConnectionTest() {

}

void ConnectionTest::test() {
    firstContact();
    userTrySendWrongData();
    multipleUsersConnect();
    longTimeWorkdTest();
}

void ConnectionTest::firstContact() {

    auto saller = makeNode(TestDataTransfer::Saller);
    auto client = makeNode(TestDataTransfer::Client);

    connectNodes(saller, client);

    QVERIFY(false);
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
    QString randomNodeName = QByteArray::number(rand()).toHex();
    auto sallerDb = QSharedPointer<RC::DataBase>::create(randomNodeName);
    sallerDb->initSqlDb();
    auto result = QSharedPointer<TestDataTransfer>::create(
                mode);


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
