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
    unsigned int userId = clientUser->userId();

    seller->setCurrentUser(sellerUser);

    // tst only last api
    seller->addApiParser<RC::ApiV1_5>();
    client->addApiParser<RC::ApiV1_5>();
    server->addApiParser<RC::ApiV1_5>();


    // run server
    QVERIFY(server->run(TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    auto obj = QSharedPointer<RC::API::UserHeader>::create();

    auto childSeller = QSharedPointer<RC::API::User>::create();
    auto contact = QSharedPointer<RC::API::Contacts>::create();

    // create child seller user
    QVERIFY(seller->createChilduser("child", childSeller, contact));

    auto updateRequest = QSharedPointer<RC::APIv1_5::UpdateContactData>::create(*contact);
    updateRequest->setRemove(false);
    updateRequest->setUserSecreet(sellerUser->secret());
    QVERIFY(seller->updateContactData(updateRequest, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    unsigned int childId = childSeller->userId();
    // wait for adding additional acces to server
    QVERIFY(wait([server, childId, userId]() {
        return server->containsContact(userId, childId);
    }, WAIT_TIME));

    // try add seals from child account
    addSeal(seller, client, server, clientUser, cardId, 6, obj);

    QVERIFY(wait([client, cardId, userId]() {
        return client->getFreeItemsCount(userId, cardId) == 1;
    }, WAIT_TIME));

    QVERIFY(seller->sentDataToServerReceive(obj, cardId, 0, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    QVERIFY(wait([client, cardId, userId]() {
        return client->getFreeItemsCount(userId, cardId) == 0;
    }, WAIT_TIME));

    updateRequest->setRemove(true);

    // try remove additional permisions from another seller account
    QVERIFY(seller->updateContactData(updateRequest, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    QVERIFY(wait([seller]() {
        return seller->getLastErrrorCode() == 1;
    }, WAIT_TIME));

    seller->setCurrentUser(sellerUser);
    // remove right of child account
    QVERIFY(seller->updateContactData(updateRequest, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    seller->setCurrentUser(childSeller);

    QVERIFY(seller->incrementPurchase(obj, cardId, 1, TEST_CHEAT_HOST, TEST_CHEAT_PORT));

    QVERIFY(wait([seller]() {
        return seller->getLastErrrorCode() == 1;
    }, WAIT_TIME));
}
