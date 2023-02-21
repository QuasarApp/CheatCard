#include "fixdbidspilltest.h"
#include "cheatcardtestshelper.h"
#include <testclient.h>
#include <QtTest>
#include <DoctorPillCore/doctortest.h>

FixDBIdsPillTest::FixDBIdsPillTest() {

}

void FixDBIdsPillTest::test() {
    auto app = CheatCardTestsHelper::makeNode<TestClient>(":/sql/units/sql/bug_386.sqlite.sql");
    QVERIFY(app);

    auto db = app->getDBObject();
    auto testedPill = db->initPills("InvalidCardIdPill");

    DP::DoctorTest test;

    QVERIFY(test.test(testedPill, true));

}
