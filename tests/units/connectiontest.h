//#
//# Copyright (C) 2020-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef EXAMPLE_TEST_H
#define EXAMPLE_TEST_H
#include "test.h"
#include "testutils.h"

#include <QtTest>

#include <CheatCard/basenode.h>


class ConnectionTest: public Test, protected TestUtils
{
public:
    ConnectionTest();
    ~ConnectionTest();

    void test();

private:
    void firstContact();
    void proxyServertests();
    void multipleUsersConnect();
    void longTimeWorkdTest();


//    void connectNodes(const QSharedPointer<TestDataTransfer>& nodeA,
//                      const QSharedPointer<TestDataTransfer>& nodeB);

};

#endif // EXAMPLE_TEST_H
