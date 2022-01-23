#include "fixdbidspilltest.h"
#include "cheatcardtestshelper.h"
#include <testseller.h>
#include <CheatCardGui/pillsselftest.h>
#include <QtTest>

FixDBIdsPillTest::FixDBIdsPillTest() {

}

void FixDBIdsPillTest::test() {
    auto app = CheatCardTestsHelper::makeNode<TestSeller>(":/sql/units/sql/bug_386.sqlite.sql");

    QVERIFY(RC::PillsSelfTest::testBadCatrdId(app->db()));

}
