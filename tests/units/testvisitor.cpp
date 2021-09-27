#include "testvisitor.h"

#include <testdatabasewrapper.h>
#include <private/card.h>
#include <private/userscards.h>

TestVisitor::TestVisitor(QSharedPointer<TestDataBaseWrapper> db): RC::Visitor(db->db()) {
        privateDb = db;
}

QSharedPointer<RC::Card> TestVisitor::getCard(unsigned int cardId) const {
    RC::Card card;
    card.setId(cardId);

    auto cardObj = db()->getObject(card);
    return cardObj;
}

int TestVisitor::getPurchaseCount(unsigned int userId, unsigned int cardId) {
    QSharedPointer<RC::UsersCards> result = getUserCardData(userId, cardId);

    if (!result)
        return 0;

    return result->getPurchasesNumber();
}
