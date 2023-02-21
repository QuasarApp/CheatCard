//#
//# Copyright (C) 2020-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include <testclient.h>
#include <CheatCard/userheader.h>
#include "api.h"
#include "contactstest.h"

#include "cheatcardtestshelper.h"
#include "testserver.h"
#include "rci/rcutils.h"

#define TEST_CHEAT_PORT 15002
#define TEST_CHEAT_HOST "localhost"

#define WAIT_FOR_FINSISHED_CONNECTION \
    QVERIFY(wait([server](){ \
        return server->connectionsCount() == 0; \
    }, WAIT_TIME));

ContactsTest::ContactsTest() {

}

void ContactsTest::test() {
    // initialise test
    QSharedPointer<TestClient> seller;
    QSharedPointer<TestClient> client;
    QSharedPointer<TestServer> server;

    qDebug() << "TEST COntacts";

    seller = CheatCardTestsHelper::makeNode<TestClient>(":/sql/units/sql/TestSallerDb.sql");
    client = CheatCardTestsHelper::makeNode<TestClient>();
    server = CheatCardTestsHelper::makeNode<TestServer>();

    QByteArray cardId = CheatCardTestsHelper::testCardId();
    auto sellerUserKey = CheatCardTestsHelper::testUserPublicKey();
    auto sellerUser = seller->getUser(sellerUserKey);
    auto clientUser = CheatCardTestsHelper::makeUser();
    auto clientUserKey = clientUser->getKey();

    client->db()->saveUser(clientUser);
    client->setCurrntUserKey(clientUserKey);

    QVERIFY(sellerUser && sellerUser->isValid());

    seller->setCurrntUserKey(sellerUserKey);

    // tst only last api
    RC::API::init({3}, seller->getDBObject(), seller.data());
    RC::API::init({3}, client->getDBObject(), client.data());
    RC::API::init({3}, server->getDBObject(), server.data());

    // run server
    QVERIFY(server->run(TEST_CHEAT_HOST, TEST_CHEAT_PORT));
    QVERIFY(client->connectToServer(TEST_CHEAT_HOST, TEST_CHEAT_PORT));
    QVERIFY(seller->connectToServer(TEST_CHEAT_HOST, TEST_CHEAT_PORT));


    auto childSeller = CheatCardTestsHelper::makeUser();
    auto childUserKey = childSeller->getKey();

    childSeller->setName("child");
    auto contact = CheatCardTestsHelper::makeContact();

    // create child seller user
    QVERIFY(RC::RCUtils::createContact(sellerUser, childSeller, contact));

    QVERIFY(seller->updateContactData(contact,
                                      sellerUser->secret(),
                                      false));

    auto childKey = childSeller->getKey();

    // wait for adding additional acces to server
    QVERIFY(wait([server, childKey, sellerUserKey]() {
        return server->containsContact(sellerUserKey, childKey);
    }, WAIT_TIME));

    WAIT_FOR_FINSISHED_CONNECTION

    seller->setCurrntUserKey(childSeller->getKey());
    seller->getDBObject()->saveUser(childSeller);
    // try add seals from child account
    addSeal(seller, client, server, clientUserKey, cardId, 6);

    QVERIFY(wait([client, cardId, clientUserKey]() {
        return client->getFreeItemsCount(clientUserKey, cardId) == 1;
    }, WAIT_TIME));

    QVERIFY(seller->incrementReceived(clientUserKey, cardId, 1));

    QVERIFY(wait([server, cardId, clientUserKey]() {
        return server->getFreeItemsCount(clientUserKey, cardId) == 0;
    }, WAIT_TIME));

    // wait for finished last seesion
    WAIT_FOR_FINSISHED_CONNECTION

    // try remove additional permisions from another seller account
    QVERIFY(seller->updateContactData(contact,
                                      childSeller->secret(),
                                      true));
    QVERIFY(wait([seller]() {
        return seller->getLastErrrorCode() == 1;
    }, WAIT_TIME));

    // wait for finished last seesion
    WAIT_FOR_FINSISHED_CONNECTION

    seller->setCurrntUserKey(sellerUserKey);
    // Remove right of child account
    QVERIFY(seller->updateContactData(contact,
                                      sellerUser->secret(),
                                      true));

    // Wait for removing additional acces to server
    QVERIFY(wait([server, childKey, sellerUserKey]() {
        return !server->containsContact(sellerUserKey, childKey);
    }, WAIT_TIME));

    // wait for finished last seesion
    WAIT_FOR_FINSISHED_CONNECTION

    seller->setCurrntUserKey(childUserKey);

    QVERIFY(seller->incrementPurchase(clientUserKey, cardId, 1));

    QVERIFY(wait([seller]() {
        return seller->getLastErrrorCode() == 1;
    }, WAIT_TIME));
}
