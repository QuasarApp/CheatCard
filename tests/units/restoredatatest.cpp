#include "CheatCard/userheader.h"
#include "api.h"
#include "cheatcardtestshelper.h"
#include "restoredatatest.h"
#include "testseller.h"
#include "testserver.h"
#include "testvisitor.h"
#include <rci/rcutils.h>

#define TEST_CHEAT_PORT 15003
#define TEST_CHEAT_HOST "localhost"

RestoreDataTest::RestoreDataTest() {

}

void RestoreDataTest::test() {


    qDebug() << "TEST RESTORE CARDS API V1";

    auto seller = CheatCardTestsHelper::makeNode<TestSeller>(":/sql/units/sql/TestSallerDb.sql");
    auto client = CheatCardTestsHelper::makeNode<TestVisitor>();
    auto server = CheatCardTestsHelper::makeNode<TestServer>();
    seller->setCurrentUser(seller->getUser(CheatCardTestsHelper::testUserId()));

    RC::API::init({2}, seller->getDBObject(), seller.data());
    RC::API::init({2}, client->getDBObject(), client.data());
    RC::API::init({2}, server->getDBObject(), server.data());

    restoreTest(seller, client, server);
}

void RestoreDataTest::restoreTest(const QSharedPointer<TestSeller> &seller,
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

    unsigned int cardId = CheatCardTestsHelper::testCardId();
    for (int i = 0; i < 6; i++) {

        qDebug () << "test case " << i << "/" << 6;

        QVERIFY(seller->incrementPurchase(obj, cardId, 1, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

        QVERIFY(wait([server, cardId](){
            auto card = server->getCard(cardId);
            return card && card->isValid();
        }, WAIT_TIME));


        QVERIFY(wait([server, user, cardId, i](){
            return server->getPurchaseCount(user->id(), cardId) == (i + 1);
        }, WAIT_TIME));

        QVERIFY(wait([server](){
            return server->connectionsCount() == 0;
        }, WAIT_TIME));

        QVERIFY(client->checkCardData(session, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

        QVERIFY(wait([client, cardId](){
            auto card = client->getCard(cardId);
            return card && card->isValid();
        }, WAIT_TIME));


        QVERIFY(wait([client, user, cardId, i](){
            return client->getPurchaseCount(user->id(), cardId) == (i + 1);
        }, WAIT_TIME));

        QVERIFY(wait([server](){
            return server->connectionsCount() == 0;
        }, WAIT_TIME));

    }

    int sellerFreeItems = seller->getFreeItemsCount(user->id(), cardId);
    int visitorFreeItems = client->getFreeItemsCount(user->id(), cardId);

    QVERIFY(sellerFreeItems == visitorFreeItems);
    QVERIFY(sellerFreeItems == 1);

    auto userKey = CheatCardTestsHelper::testUserPublicKey();
    auto testPrivKey = CheatCardTestsHelper::testUserPrivKey();

    QVERIFY(RC::RCUtils::makeUserKey(testPrivKey) == userKey);
    QVERIFY(RC::RCUtils::makeUserId(userKey) == CheatCardTestsHelper::testUserId());

    seller->dropDB();

    // check card after clear. card should be removed
    QVERIFY(wait([seller, cardId](){
        auto cardSeller = seller->getCard(cardId);
        return !cardSeller;
    }, WAIT_TIME));

    QVERIFY(seller->restoreAllData(user->getKey(), TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    // check card after restore. card should be exits
    QVERIFY(wait([seller, cardId](){
        auto cardSeller = seller->getCard(cardId);
        return cardSeller && cardSeller->isValid();
    }, WAIT_TIME));
}
