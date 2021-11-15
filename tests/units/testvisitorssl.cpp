#include "testvisitorssl.h"

#include <testdatabasewrapper.h>
#include <CheatCard/card.h>
#include <CheatCard/userscards.h>

TestVisitorSSL::TestVisitorSSL(QSharedPointer<TestDataBaseWrapper> db):
    RC::VisitorSSL(db->db()) {
        privateDb = db;
        setRequestInterval(0);
}

QSharedPointer<RC::Card> TestVisitorSSL::getCard(unsigned int cardId) const {
    RC::Card card;
    card.setId(cardId);

    auto cardObj = db()->getObject(card);
    return cardObj;
}

int TestVisitorSSL::getPurchaseCount(unsigned int userId, unsigned int cardId) {
    QSharedPointer<RC::UsersCards> result = getUserCardData(userId, cardId);

    if (!result)
        return 0;

    return result->getPurchasesNumber();
}
