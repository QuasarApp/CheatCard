#include "securitytest.h"

#include "CheatCard/userheader.h"
#include "api.h"
#include "testserver.h"
#include <testseller.h>
#include <testvisitor.h>
#include "cheatcardtestshelper.h"


#define TEST_CHEAT_PORT 15004
#define TEST_CHEAT_HOST "localhost"

SecurityTest::SecurityTest() {

}

SecurityTest::~SecurityTest() {

}

void SecurityTest::test() {
    QSharedPointer<TestSeller> seller;
    QSharedPointer<TestVisitor> client;
    QSharedPointer<TestServer> server;

    qDebug() << "TEST API V1";

    seller = CheatCardTestsHelper::makeNode<TestSeller>(":/sql/units/sql/TestSallerDb.sql");

    client = CheatCardTestsHelper::makeNode<TestVisitor>();

    server = CheatCardTestsHelper::makeNode<TestServer>();
    seller->setCurrentUser(seller->getUser(CheatCardTestsHelper::testUserId()));

    RC::API::init({2}, seller->getDBObject(), seller.data());
    RC::API::init({2}, client->getDBObject(), client.data());
    RC::API::init({2}, server->getDBObject(), server.data());

    secureTest(seller, client, server);

}

void SecurityTest::secureTest(const QSharedPointer<TestSeller> &seller,
                              const QSharedPointer<TestVisitor> &client,
                              const QSharedPointer<TestServer> &server) {

    // random session
    long long session = rand() * rand();

    // run server
    QVERIFY(server->run(TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    auto user = CheatCardTestsHelper::makeUser();
    auto obj = QSharedPointer<RC::UserHeader>::create();

    obj->setSessionId(session);
    obj->setToken(user->getKey());
    obj->setUserId(user->id());

    // 3619648333 This is card id from test database.
    unsigned int cardId = CheatCardTestsHelper::testCardId();

    addSeal(seller, client, server, user, cardId, 1, obj, TEST_CHEAT_HOST, TEST_CHEAT_PORT);

    // make another sellre with some card.
    auto seller2 = CheatCardTestsHelper::makeNode<TestSeller>(":/sql/units/sql/TestSallerDb.sql");
    auto newSellerUser = CheatCardTestsHelper::makeUser();

    seller2->setCurrentUser(newSellerUser);
    seller2->getDBObject()->saveUser(newSellerUser);

    RC::API::init({2}, seller2->getDBObject(), seller2.data());


    obj->setSessionId(rand() * rand());

    QVERIFY(seller2->incrementPurchase(obj, cardId, 10, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    QVERIFY(wait([seller2]() {
        // 1 - worong command
        return seller2->getLastErrrorCode() == 1;
    }, WAIT_TIME));

}
