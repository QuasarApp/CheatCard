#include "testvisitor.h"


TestVisitor::TestVisitor(const QSharedPointer<RC::Interfaces::iDB> &db):
    RC::Visitor(db) {
        setRequestInterval(0);
}

QSharedPointer<RC::Interfaces::iCard> TestVisitor::getCard(unsigned int cardId) const {
    return db()->getCard(cardId);
}

int TestVisitor::getPurchaseCount(unsigned int userId, unsigned int cardId) {
    auto result = db()->getUserCardData(userId, cardId);

    if (!result)
        return 0;

    return result->getPurchasesNumber();
}

int TestVisitor::getFreeItemsCount(unsigned int userId, unsigned int cardId) {
    auto dbUsersCards = db()->getUserCardData(
        userId,
        cardId);
    return db()->getFreeItemsCount(dbUsersCards);
}

const QSharedPointer<RC::Interfaces::iDB> &TestVisitor::getDBObject() const {
    return db();
}

void TestVisitor::dropDB() {
    db()->clearOldData(-1);
}
