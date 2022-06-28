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

    seller->addApiParser<RC::ApiV1>();
    client->addApiParser<RC::ApiV1>();
    server->addApiParser<RC::ApiV1>();
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
    unsigned int userId = user->userId();


    int sellerFreeItems = seller->getFreeItemsCount(user->userId(), cardId);
    int visitorFreeItems = client->getFreeItemsCount(user->userId(), cardId);

    QVERIFY(sellerFreeItems == visitorFreeItems);
    QVERIFY(sellerFreeItems == 16);

}
