#include "securitytest.h"

#include "testserver.h"
#include <testseller.h>
#include <testvisitor.h>
#include "cheatcardtestshelper.h"
#include <CheatCard/api/apiv1-5.h>
#include <CheatCard/api/api0/userheader.h>
#include <CheatCard/api/api0/user.h>
#include <CheatCard/api/api0/card.h>

#define TEST_CHEAT_PORT 15001
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

    seller = CheatCardTestsHelper::makeNode<TestSeller>();

    client = CheatCardTestsHelper::makeNode<TestVisitor>();

    server = CheatCardTestsHelper::makeNode<TestServer>();
    seller->setCurrentUser(seller->getUser(CheatCardTestsHelper::testUserId()));

    seller->addApiParser<RC::ApiV1_5>();
    client->addApiParser<RC::ApiV1_5>();
    server->addApiParser<RC::ApiV1_5>();
    server->addApiParser<RC::ApiV1_5>();

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
    auto obj = QSharedPointer<RC::API::UserHeader>::create();

    obj->setSessionId(session);
    obj->setToken(user->getKey());
    obj->setUserId(user->userId());

    // 3619648333 This is card id from test database.
    unsigned int cardId = CheatCardTestsHelper::testCardId();

    addSeal(seller, client, server, user, cardId, 1, obj);

    // make another sellre with some card.
    auto seller2 = CheatCardTestsHelper::makeNode<TestSeller>();
    auto newSellerUser = CheatCardTestsHelper::makeUser();
    seller2->setCurrentUser(newSellerUser);
    seller2->addApiParser<RC::ApiV1_5>();


    obj->setSessionId(rand() * rand());
    QVERIFY(seller2->incrementPurchase(obj, cardId, 10, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    QVERIFY(wait([seller2]() {
        // 1 - worong command
        return seller2->getLastErrrorCode() == 1;
    }, WAIT_TIME));

}
