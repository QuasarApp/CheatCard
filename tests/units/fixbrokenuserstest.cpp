#include "fixbrokenuserstest.h"
#include "cheatcardtestshelper.h"
#include <testclient.h>
#include <QtTest>
#include <DoctorPillCore/doctortest.h>
#include <QCryptographicHash>

FixBrokenUsersTest::FixBrokenUsersTest() {

}

void FixBrokenUsersTest::test() {
    auto app = CheatCardTestsHelper::makeNode<TestClient>({3});
    QVERIFY(app);

    auto db = app->getDBObject();
    auto testedPill = db->initPills("InvalidUsersPill");

    // create a invalid user.
    auto user = db->makeEmptyUser();
    QVERIFY(user && user->isValid());
    user->setSecret(QCryptographicHash::hash("wrong key", QCryptographicHash::Sha256));
    QVERIFY(user->isValid());

    QVERIFY(db->saveUser(user));

    DP::DoctorTest test;

    QVERIFY(test.test(testedPill, true));
}
