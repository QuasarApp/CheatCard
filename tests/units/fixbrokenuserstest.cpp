#include "fixbrokenuserstest.h"
#include "cheatcardtestshelper.h"
#include <testclient.h>
#include <QtTest>
#include <DoctorPillCore/doctortest.h>


FixBrokenUsersTest::FixBrokenUsersTest() {

}

void FixBrokenUsersTest::test() {
    auto app = CheatCardTestsHelper::makeNode<TestClient>(":/sql/units/sql/bug_539.sqlite.sql");
    QVERIFY(app);

    auto db = app->getDBObject();
    auto testedPill = db->initPills("InvalidUsersPill");

    DP::DoctorTest test;

    QVERIFY(test.test(testedPill, true));
}
