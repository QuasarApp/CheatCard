#include "fixbrokenuserstest.h"
#include "cheatcardtestshelper.h"
#include <testseller.h>
#include <QtTest>
#include <DoctorPillCore/doctortest.h>
#include <CheatCard/pills/invaliduserspill.h>


FixBrokenUsersTest::FixBrokenUsersTest() {

}

void FixBrokenUsersTest::test() {
    auto app = CheatCardTestsHelper::makeNode<TestSeller>(":/sql/units/sql/bug_539.sqlite.sql");
    QVERIFY(app);
    DP::DoctorTest test;

    QVERIFY(test.test({QSharedPointer<RC::InvalidUsersPill>::create(app->db())}, true));

}
