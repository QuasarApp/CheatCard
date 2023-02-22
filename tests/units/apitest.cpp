#include "apitest.h"
#include "testclient.h"
#include "testutils.h"
#include "cheatcardtestshelper.h"

#define TEST_CHEAT_PORT 15005
#define TEST_CHEAT_HOST "localhost"

template<class Base>
class ApiTester: public Base {
public:
    ApiTester(RC::BaseNode* node): Base(node) {}
    friend class APITest;
};

APITest::APITest() {

}

void APITest::test() {
    testProcessDeleteCardRequest();
}

void APITest::testProcessDeleteCardRequest() {
    auto network = CheatCardTestsHelper::deployNetwork(TEST_CHEAT_HOST, TEST_CHEAT_PORT);
    QVERIFY(network.clients.count() == 2);

    auto card = CheatCardTestsHelper::makeCard(network.clients.begin().value(), 10);
    auto seller = *network.clients.begin();
    auto client = *std::next(network.clients.begin());

    CheatCardTestsHelper::makeSeals(seller, client, card->cardId(), 30);

    seller->incrementReceived(client->currntUserKey(), card->cardId(), 2);
    QVERIFY(TestUtils::wait([client, card]() {
        return client->getFreeItemsCount(client->currntUserKey(), card->cardId()) == 1;
    }, WAIT_TIME));
}


