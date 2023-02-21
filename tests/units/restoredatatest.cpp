#include "api.h"
#include "cheatcardtestshelper.h"
#include "restoredatatest.h"
#include "testclient.h"
#include "testserver.h"
#include <rci/rcutils.h>

#define TEST_CHEAT_PORT 15003
#define TEST_CHEAT_HOST "localhost"

RestoreDataTest::RestoreDataTest() {

}

void RestoreDataTest::test() {


    qDebug() << "TEST RESTORE CARDS API";

    auto seller = CheatCardTestsHelper::makeNode<TestClient>(":/sql/units/sql/TestSallerDb.sql");
    auto client = CheatCardTestsHelper::makeNode<TestClient>();
    auto server = CheatCardTestsHelper::makeNode<TestServer>();
    seller->setCurrntUserKey(CheatCardTestsHelper::testUserPublicKey());

    RC::API::init({3}, seller->getDBObject(), seller.data());
    RC::API::init({3}, client->getDBObject(), client.data());
    RC::API::init({3}, server->getDBObject(), server.data());

    restoreTest(seller, client, server);
}

void RestoreDataTest::restoreTest(const QSharedPointer<TestClient> &seller,
                                  const QSharedPointer<TestClient> &client,
                                  const QSharedPointer<TestServer> &server) {

    // run server
    QVERIFY(server->run(TEST_CHEAT_HOST, TEST_CHEAT_PORT));
    QVERIFY(seller->connectToServer(TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    auto user = CheatCardTestsHelper::makeUser();

    QByteArray cardId = CheatCardTestsHelper::testCardId();

    addSeal(seller, client, server, user->getKey(), cardId, 6);

    int sellerFreeItems = seller->getFreeItemsCount(user->getKey(), cardId);
    int visitorFreeItems = client->getFreeItemsCount(user->getKey(), cardId);

    QVERIFY(sellerFreeItems == visitorFreeItems);
    QVERIFY(sellerFreeItems == 1);

    auto userKey = CheatCardTestsHelper::testUserPublicKey();
    auto testPrivKey = CheatCardTestsHelper::testUserPrivKey();

    QVERIFY(RC::RCUtils::makeUserKey(testPrivKey) == userKey);

    seller->dropDB();

    // check card after clear. card should be removed
    QVERIFY(wait([seller, cardId](){
        auto cardSeller = seller->getCard(cardId);
        return !cardSeller;
    }, WAIT_TIME));

    QVERIFY(seller->subscribeToUser(user->getKey()));

    // check card after restore. card should be exits
    QVERIFY(wait([seller, cardId](){
        auto cardSeller = seller->getCard(cardId);
        return cardSeller && cardSeller->isValid();
    }, WAIT_TIME));
}
