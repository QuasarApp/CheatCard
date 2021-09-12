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
