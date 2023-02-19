#include "apitest.h"
#include "api.h"
#include "testclient.h"
#include "testserver.h"
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
    // intialization nodes.
    auto server = CheatCardTestsHelper::makeNode<TestServer>();
    auto seller1 = CheatCardTestsHelper::makeNode<TestClient>();
    auto seller2 = CheatCardTestsHelper::makeNode<TestClient>();

    // initialization api objects for nodes.
    RC::API::init({2}, server->getDBObject(), server.data());
    RC::API::init({2}, seller1->getDBObject(), seller1.data());
    RC::API::init({2}, seller2->getDBObject(), seller2.data());

    // run server node.
    QVERIFY(server->run(TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    QVERIFY(seller1->connectToServer(TEST_CHEAT_HOST, TEST_CHEAT_PORT));
    QVERIFY(seller2->connectToServer(TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    // init meta objects. (create card)
    auto seller1DB = seller1->getDBObject();
    auto seller2DB = seller2->getDBObject();

    auto card = seller1DB->makeEmptyCard();
    QByteArray cardId = card->cardId();

    // initialisation of the first user object.
    auto seller1User = seller1DB->makeEmptyUser();
    QVERIFY(seller1User->isValid());
    QVERIFY(seller1DB->saveUser(seller1User));

    // initialisation of the second user object.
    auto seller2User = seller2DB->makeEmptyUser();
    QVERIFY(seller2User->isValid());
    QVERIFY(seller2DB->saveUser(seller2User));

    // create a test card
    card->setTitle("testCard");
    card->setFreeIndex(10);
    card->setOwnerSignature(seller1User->getKey());
    card->setFreeItemName("Free Item");

    QVERIFY(card->isValid());
    QVERIFY(seller1DB->saveCard(card));

    // send card to server.
    QVERIFY(seller1->updateCard(cardId, card->getCardVersion() + 1));
    QVERIFY(wait([server, cardId](){
        auto card = server->getCard(cardId);
        return card && card->isValid();
    }, WAIT_TIME));

    // make a cheat client with random secret
    seller1User->setSecret(QCryptographicHash::hash("secret", QCryptographicHash::Sha256));
    QVERIFY(seller2DB->saveUser(seller1User));
    QVERIFY(seller2DB->saveCard(card));

    seller2->setCurrntUserKey(seller1User->getKey());

    // send request tu remove (shold be failed and client shld be receivve error message.)
    QVERIFY(seller2->deleteCard(cardId));
    QVERIFY(wait([seller2](){
        auto err = seller2->getLastErrrorCode();
        return err == 1;
    }, WAIT_TIME));


    // card should be exists
    QVERIFY(server->getCard(cardId));


    seller1->setCurrntUserKey(seller1User->getKey());
    QVERIFY(seller1->deleteCard(cardId));

    // wait for delete card on server
    QVERIFY(wait([server, cardId](){
        return !server->getCard(cardId);
    }, WAIT_TIME));



}


