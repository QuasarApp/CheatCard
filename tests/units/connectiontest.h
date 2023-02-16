//#
//# Copyright (C) 2020-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef EXAMPLE_TEST_H
#define EXAMPLE_TEST_H
#include "test.h"
#include "testutils.h"

#include <QtTest>

#include <CheatCard/basenode.h>

namespace RC {
namespace Intefaces {
    class iUser;
}
class UserHeader;
}

class TestClient;
class TestServer;

/**
 * @brief The ConnectionTest class check connections betwin nodes
 */
class ConnectionTest: public Test, protected TestUtils
{
public:
    ConnectionTest();
    ~ConnectionTest();

    void test();

protected:

    void addSeal(const QSharedPointer<TestClient> &seller,
                 const QSharedPointer<TestClient> &client,
                 const QSharedPointer<TestServer> &server,
                 const QByteArray &userKey,
                 const QByteArray &cardId, int sealsCount);
private:
    void firstContact();
    void apiTest(const QSharedPointer<TestClient> &seller,
                 const QSharedPointer<TestClient> &client,
                 const QSharedPointer<TestServer> &server);

};

#endif // EXAMPLE_TEST_H
