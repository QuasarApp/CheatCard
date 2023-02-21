//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef RESTOREDATATEST_H
#define RESTOREDATATEST_H

#include <QtTest>
#include <connectiontest.h>


class TestClient;
class TestServer;

class RestoreDataTest:  public ConnectionTest
{
public:
    RestoreDataTest();
    void test();

private:
    void restoreTest(const QSharedPointer<TestClient> &seller,
                     const QSharedPointer<TestClient> &client,
                     const QSharedPointer<TestServer> &server);

};

#endif // RESTOREDATATEST_H
