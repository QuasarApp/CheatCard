#include "testdatabasewrapper.h"
#include "testserver.h"

#include <CheatCard/api/api0/card.h>
#include <CheatCard/api/api0/userscards.h>

#include <QCoreApplication>

TestServer::TestServer(QSharedPointer<TestDataBaseWrapper> db):
    RC::Server(db->db()) {
    privateDb = db;
}

QSharedPointer<RC::API::Card> TestServer::getCard(unsigned int cardId) const {
    RC::API::Card card;
    card.setId(cardId);

    auto cardObj = db()->getObject(card);
    return cardObj;
}

int TestServer::getPurchaseCount(unsigned int userId, unsigned int cardId) {
    QSharedPointer<RC::API::UsersCards> result = getUserCardData(userId, cardId);

    if (!result)
        return 0;

    return result->getPurchasesNumber();
}
