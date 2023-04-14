#include "cheatcardtestshelper.h"
#include "restoredatatest.h"
#include "testclient.h"
#include <rci/rcutils.h>
#include "testutils.h"

#define TEST_CHEAT_PORT 15003
#define TEST_CHEAT_HOST "localhost"

RestoreDataTest::RestoreDataTest() {

}

void RestoreDataTest::test() {


    auto network = CheatCardTestsHelper::deployNetwork(TEST_CHEAT_HOST, TEST_CHEAT_PORT, 2, false);
    QVERIFY(network.clients.count() == 2);

    auto seller = *network.clients.begin();
    auto client = *std::next(network.clients.begin());

    QVERIFY(seller->connectToServer(TEST_CHEAT_HOST, TEST_CHEAT_PORT));
    QVERIFY(TestUtils::wait([seller]() {
            return seller->isSynced();
    }, WAIT_TIME));

    auto card = CheatCardTestsHelper::makeCard(network.clients.begin().value(), 10);

    CheatCardTestsHelper::makeSealsFor(seller, client->currntUserKey(), card->cardId(), 30);
    seller->incrementReceived(client->currntUserKey(), card->cardId(), 2);

    QVERIFY(client->getFreeItemsCount(client->currntUserKey(), card->cardId()) == 0);

    QVERIFY(client->connectToServer(TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    QVERIFY(TestUtils::wait([client, card]() {
        return client->getFreeItemsCount(client->currntUserKey(), card->cardId()) == 1;
    }, WAIT_TIME));

}
