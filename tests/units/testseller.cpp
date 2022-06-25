#include "testdatabasewrapper.h"
#include "testseller.h"
#include <CheatCard/api/api0/userscards.h>

#include <CheatCard/clearolddata.h>

TestSeller::TestSeller(QSharedPointer<TestDataBaseWrapper> db): RC::Seller(db->db()) {
    privateDb = db;
}

int TestSeller::getPurchaseCount(unsigned int userId, unsigned int cardId) {
    QSharedPointer<RC::API::UsersCards> result = getUserCardData(userId, cardId);

    if (!result)
        return 0;

    return result->getPurchasesNumber();
}

void TestSeller::dropDB() {

    // run force clear
    auto task = QSharedPointer<RC::ClearOldData>::create(-1);
    task->setTime(0);
    task->setMode(QH::ScheduleMode::SingleWork);
    sheduleTask(task);
}
