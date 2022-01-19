#include "fixdbidspilltest.h"
#include "cheatcardtestshelper.h"
#include <testseller.h>

FixDBIdsPillTest::FixDBIdsPillTest() {

}

void FixDBIdsPillTest::test() {
    auto app = CheatCardTestsHelper::makeNode<TestSeller>("qrc:/sql/units/sql/bug_386.sqlite.sql");


}
