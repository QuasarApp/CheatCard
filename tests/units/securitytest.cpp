#include "securitytest.h"

#include "qtestcase.h"
#include "testclient.h"
#include "testutils.h"

#include "cheatcardtestshelper.h"


#define TEST_CHEAT_PORT 15004
#define TEST_CHEAT_HOST "localhost"

SecurityTest::SecurityTest() {
    auto network = CheatCardTestsHelper::deployNetwork(TEST_CHEAT_HOST, TEST_CHEAT_PORT, 3);
    QVERIFY(network.clients.count() == 3);

    auto card = CheatCardTestsHelper::makeCard(network.clients.begin().value(), 10);
    auto seller = *network.clients.begin();
    auto client = *std::next(network.clients.begin());
    auto badseller = *std::next(network.clients.begin(), 2);

    CheatCardTestsHelper::makeSealsFor(seller, client->currntUserKey(), card->cardId(), 30);

    seller->incrementReceived(client->currntUserKey(), card->cardId(), 2);
    QVERIFY(TestUtils::wait([client, card]() {
        return client->getFreeItemsCount(client->currntUserKey(), card->cardId()) == 1;
    }, WAIT_TIME));


    CheatCardTestsHelper::checkAccess(seller, client->currntUserKey(), card->cardId(), true);
    CheatCardTestsHelper::checkAccess(badseller, client->currntUserKey(), card->cardId(), false);

}

SecurityTest::~SecurityTest() {

}

void SecurityTest::test() {

}
