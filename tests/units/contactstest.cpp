//#
//# Copyright (C) 2020-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include <testseller.h>
#include <testvisitor.h>
#include "contactstest.h"

#include <CheatCard/api/api0/card.h>
#include <CheatCard/api/api0/user.h>
#include <CheatCard/api/api0/userheader.h>
#include "cheatcardtestshelper.h"
#include "testserver.h"
#include <CheatCard/api/apiv1-5.h>
#include <CheatCard/api/api1-5/updatecontactdata.h>

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
    QSharedPointer<TestSeller> seller;
    QSharedPointer<TestVisitor> client;
    QSharedPointer<TestServer> server;

    qDebug() << "TEST API V1";

    seller = CheatCardTestsHelper::makeNode<TestSeller>();
    client = CheatCardTestsHelper::makeNode<TestVisitor>();
    server = CheatCardTestsHelper::makeNode<TestServer>();
    auto sellerUser = seller->getUser(CheatCardTestsHelper::testUserId());
    auto clientUser = CheatCardTestsHelper::makeUser();
    unsigned int cardId = CheatCardTestsHelper::testCardId();
    auto sellerUserKey = sellerUser->getKey();

    unsigned int clientUserId = clientUser->userId();

    seller->setCurrentUser(sellerUser);

    // tst only last api
    seller->addApiParser<RC::ApiV1_5>();
    client->addApiParser<RC::ApiV1_5>();
    server->addApiParser<RC::ApiV1_5>();


    // run server
    QVERIFY(server->run(TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    auto obj = QSharedPointer<RC::API::UserHeader>::create();

    auto childSeller = QSharedPointer<RC::API::User>::create();
    childSeller->setName("child");
    auto contact = QSharedPointer<RC::API::Contacts>::create();

    // create child seller user
    QVERIFY(seller->createContact(childSeller, contact));

    QVERIFY(seller->updateContactData(*contact,
                                      sellerUser->secret(),
                                      false,
                                      TEST_CHEAT_HOST,
                                      TEST_CHEAT_PORT));

    auto childKey = childSeller->getKey();

    // wait for adding additional acces to server
    QVERIFY(wait([server, childKey, sellerUserKey]() {
        return server->containsContact(sellerUserKey, childKey);
    }, WAIT_TIME));

    WAIT_FOR_FINSISHED_CONNECTION

    seller->setCurrentUser(childSeller);
    // try add seals from child account
    addSeal(seller, client, server, clientUser, cardId, 6, obj, TEST_CHEAT_HOST,  TEST_CHEAT_PORT);

    QVERIFY(wait([client, cardId, clientUserId]() {
        return client->getFreeItemsCount(clientUserId, cardId) == 1;
    }, WAIT_TIME));

    QVERIFY(seller->sentDataToServerReceive(obj, cardId, 1, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    QVERIFY(wait([server, cardId, clientUserId]() {
        return server->getFreeItemsCount(clientUserId, cardId) == 0;
    }, WAIT_TIME));

    // wait for finished last seesion
    WAIT_FOR_FINSISHED_CONNECTION

    // try remove additional permisions from another seller account
    QVERIFY(seller->updateContactData(*contact,
                                      childSeller->secret(),
                                      true,
                                      TEST_CHEAT_HOST,
                                      TEST_CHEAT_PORT));
    QVERIFY(wait([seller]() {
        return seller->getLastErrrorCode() == 1;
    }, WAIT_TIME));

    // wait for finished last seesion
    WAIT_FOR_FINSISHED_CONNECTION

    seller->setCurrentUser(sellerUser);
    // Remove right of child account
    QVERIFY(seller->updateContactData(*contact,
                                      sellerUser->secret(),
                                      true,
                                      TEST_CHEAT_HOST,
                                      TEST_CHEAT_PORT));

    // Wait for removing additional acces to server
    QVERIFY(wait([server, childKey, sellerUserKey]() {
        return !server->containsContact(sellerUserKey, childKey);
    }, WAIT_TIME));

    // wait for finished last seesion
    WAIT_FOR_FINSISHED_CONNECTION

    seller->setCurrentUser(childSeller);

    QVERIFY(seller->incrementPurchase(obj, cardId, 1, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    QVERIFY(wait([seller]() {
        return seller->getLastErrrorCode() == 1;
    }, WAIT_TIME));
}
