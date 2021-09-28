#include "testdatabasewrapper.h"
#include "testserver.h"

#include <CheatCard/card.h>
#include <CheatCard/userscards.h>

TestServer::TestServer(QSharedPointer<TestDataBaseWrapper> db): RC::Server(db->db()) {
    privateDb = db;
}

QSharedPointer<RC::Card> TestServer::getCard(unsigned int cardId) const {
    RC::Card card;
    card.setId(cardId);

    auto cardObj = db()->getObject(card);
    return cardObj;
}

int TestServer::getPurchaseCount(unsigned int userId, unsigned int cardId) {
    QSharedPointer<RC::UsersCards> result = getUserCardData(userId, cardId);

    if (!result)
        return 0;

    return result->getPurchasesNumber();
}
