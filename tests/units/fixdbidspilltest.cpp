#include "fixdbidspilltest.h"
#include "cheatcardtestshelper.h"
#include <testseller.h>
#include <QtTest>
#include <DoctorPillCore/doctortest.h>
#include <CheatCard/pills/invalidcardidpill.h>

FixDBIdsPillTest::FixDBIdsPillTest() {

}

void FixDBIdsPillTest::test() {
    auto app = CheatCardTestsHelper::makeNode<TestSeller>(":/sql/units/sql/bug_386.sqlite.sql");
    QVERIFY(app);
    DP::DoctorTest test;

    QVERIFY(test.test({QSharedPointer<RC::InvalidCardIdPill>::create(app->db())}, true));

}
