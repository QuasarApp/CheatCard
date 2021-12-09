#include "cheatcardtestshelper.h"
#include "restoredatatest.h"
#include "testseller.h"
#include "testserver.h"
#include "testvisitor.h"

#include <CheatCard/api/api0/card.h>
#include <CheatCard/api/api0/user.h>
#include <CheatCard/api/api0/userheader.h>
#include <CheatCard/api/apiv1.h>


#define TEST_CHEAT_PORT 15001
#define TEST_CHEAT_HOST "localhost"

RestoreDataTest::RestoreDataTest() {

}

void RestoreDataTest::test() {


    qDebug() << "TEST RESTORE CARDS API V1";

    auto seller = CheatCardTestsHelper::makeNode<TestSeller>();
    auto client = CheatCardTestsHelper::makeNode<TestVisitor>();
    auto server = CheatCardTestsHelper::makeNode<TestServer>();
    seller->setCurrentUser(seller->getUser(CheatCardTestsHelper::testUserId()));

    seller->addApiParser<RC::ApiV1>();
    client->addApiParser<RC::ApiV1>();
    server->addApiParser<RC::ApiV1>();

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
    auto obj = QSharedPointer<RC::API::UserHeader>::create();

    obj->setSessionId(session);
    obj->setToken(user->getKey());
    obj->setUserId(user->userId());

    unsigned int cardId = CheatCardTestsHelper::testCardId();
    for (int i = 0; i < 6; i++) {

        qDebug () << "test case " << i << "/" << 6;

        QVERIFY(seller->incrementPurchase(obj, cardId, 1, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

        QVERIFY(wait([server, cardId](){
            auto card = server->getCard(cardId);
            return card && card->isValid();
        }, WAIT_TIME));


        QVERIFY(wait([server, user, cardId, i](){
            return server->getPurchaseCount(user->userId(), cardId) == (i + 1);
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
            return client->getPurchaseCount(user->userId(), cardId) == (i + 1);
        }, WAIT_TIME));

        QVERIFY(wait([server](){
            return server->connectionsCount() == 0;
        }, WAIT_TIME));

    }

    int sellerFreeItems = seller->getFreeItemsCount(user->userId(), cardId);
    int visitorFreeItems = client->getFreeItemsCount(user->userId(), cardId);

    QVERIFY(sellerFreeItems == visitorFreeItems);
    QVERIFY(sellerFreeItems == 1);

    auto userKey = CheatCardTestsHelper::testUserPublicKey();
    auto testPrivKey = CheatCardTestsHelper::testUserPrivKey();

    QVERIFY(RC::API::User::makeKey(testPrivKey) == userKey);
    QVERIFY(RC::API::User::makeId(userKey) == CheatCardTestsHelper::testUserId());

    seller->dropDB();

    // check card after clear. card should be removed
    QVERIFY(wait([seller, cardId](){
        auto cardSeller = seller->getCard(cardId);
        return !cardSeller;
    }, WAIT_TIME));

    QVERIFY(seller->restoreOldData(user->getKey(), TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    // check card after restore. card should be exits
    QVERIFY(wait([seller, cardId](){
        auto cardSeller = seller->getCard(cardId);
        return cardSeller && cardSeller->isValid();
    }, WAIT_TIME));
}
