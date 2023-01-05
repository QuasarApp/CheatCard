//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef RESTOREDATATEST_H
#define RESTOREDATATEST_H


#include "test.h"
#include "testutils.h"

#include <QtTest>

#include <CheatCard/basenode.h>


class TestSeller;
class TestVisitor;
class TestServer;

class RestoreDataTest: public Test, protected TestUtils
{
public:
    RestoreDataTest();
    void test();

private:
    void restoreTest(const QSharedPointer<TestSeller> &seller,
                     const QSharedPointer<TestVisitor> &client,
                     const QSharedPointer<TestServer> &server);

};

#endif // RESTOREDATATEST_H
