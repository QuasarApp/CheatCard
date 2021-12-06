#include "testdatabasewrapper.h"
#include "testseller.h"

#include <CheatCard/clearolddata.h>

TestSeller::TestSeller(QSharedPointer<TestDataBaseWrapper> db): RC::Seller(db->db()) {
    privateDb = db;
}

void TestSeller::dropDB() {

    // run force clear
    auto task = QSharedPointer<RC::ClearOldData>::create(-1);
    task->setTime(0);
    task->setMode(QH::ScheduleMode::SingleWork);
    sheduleTask(task);
}
