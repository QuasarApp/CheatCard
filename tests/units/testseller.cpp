#include "testseller.h"

#include <CheatCard/clearolddata.h>

TestSeller::TestSeller(const QSharedPointer<RC::Interfaces::iDB> &db): RC::Seller(db) {
    connect (this, &TestSeller::requestError, this, &TestSeller::handleRequestError);
}

int TestSeller::getPurchaseCount(unsigned int userId, unsigned int cardId) {
    auto result = db()->getUserCardData(userId, cardId);

    if (!result)
        return 0;

    return result->getPurchasesNumber();
}

int TestSeller::getFreeItemsCount(unsigned int userId, unsigned int cardId) {
    auto dbUsersCards = db()->getUserCardData(
        userId,
        cardId);
    return db()->getFreeItemsCount(dbUsersCards);
}

QSharedPointer<RC::Interfaces::iCard> TestSeller::getCard(unsigned int cardId) const {
    return db()->getCard(cardId);
}

void TestSeller::handleRequestError(unsigned char code, QString) {
    lastErrrorCode = code;
}

unsigned char TestSeller::getLastErrrorCode() {
    unsigned char lastErr = lastErrrorCode;
    lastErrrorCode = 0;
    return lastErr;
}

const QSharedPointer<RC::Interfaces::iDB> &TestSeller::getDBObject() const {
    return db();
}

QSharedPointer<RC::Interfaces::iUser>
TestSeller::getUser(unsigned int userId) const {
    return db()->getUser(userId);
}

void TestSeller::dropDB() {

    // run force clear
    auto task = QSharedPointer<RC::ClearOldData>::create(db(), -1);
    task->setTime(0);
    task->setMode(QH::ScheduleMode::SingleWork);
    sheduleTask(task);
}
