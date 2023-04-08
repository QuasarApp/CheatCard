#include "securitytest.h"

#include "qtestcase.h"
#include "testclient.h"
#include "testutils.h"

#include "cheatcardtestshelper.h"


#define TEST_CHEAT_PORT 15004
#define TEST_CHEAT_HOST "localhost"

SecurityTest::SecurityTest() {

}

SecurityTest::~SecurityTest() {

}

void SecurityTest::test() {
    auto network = CheatCardTestsHelper::deployNetwork(TEST_CHEAT_HOST, TEST_CHEAT_PORT, 4);
    QVERIFY(network.clients.count() == 4);

    auto seller = *network.clients.begin();
    auto card = CheatCardTestsHelper::makeCard(seller, 10);

    auto client = *std::next(network.clients.begin());
    auto badseller = *std::next(network.clients.begin(), 2);
    auto childSeller = *std::next(network.clients.begin(), 3);

    // all nodes should be know about clinet all
    badseller->subscribeToUser(client->currntUserKey());
    childSeller->subscribeToUser(client->currntUserKey());

    CheatCardTestsHelper::makeSealsFor(seller, client->currntUserKey(), card->cardId(), 30);

    seller->incrementReceived(client->currntUserKey(), card->cardId(), 2);
    QVERIFY(TestUtils::wait([client, card]() {
        return client->getFreeItemsCount(client->currntUserKey(), card->cardId()) == 1;
    }, WAIT_TIME));


    CheatCardTestsHelper::checkAccess(seller, network.server, client->currntUserKey(), card->cardId(), true);
    CheatCardTestsHelper::checkAccess(badseller, network.server, client->currntUserKey(), card->cardId(), false);
    CheatCardTestsHelper::checkAccess(childSeller, network.server, client->currntUserKey(), card->cardId(), false);

    QVERIFY(seller->grantAccess(seller->getCurrentUser(), childSeller->currntUserKey()));
    QVERIFY(TestUtils::wait([seller, childSeller]() {
        return childSeller->getContact(seller->currntUserKey(), childSeller->currntUserKey());
    }, WAIT_TIME));


    CheatCardTestsHelper::checkAccess(childSeller, network.server, client->currntUserKey(), card->cardId(), true);

    QVERIFY(seller->dropAccess(seller->getCurrentUser(), childSeller->currntUserKey()));
    QVERIFY(TestUtils::wait([seller, childSeller]() {
        return !childSeller->getContact(seller->currntUserKey(), childSeller->currntUserKey());
    }, WAIT_TIME));

    CheatCardTestsHelper::checkAccess(childSeller, network.server, client->currntUserKey(), card->cardId(), false);

    QVERIFY(client->getPurchaseCount(client->currntUserKey(), card->cardId()) == 32);
}
