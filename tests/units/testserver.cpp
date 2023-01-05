#include "testserver.h"

#include <QCoreApplication>

TestServer::TestServer(const QSharedPointer<RC::Interfaces::iDB> &db):
    RC::Server(db) {
}

QSharedPointer<RC::Interfaces::iCard> TestServer::getCard(unsigned int cardId) const {

    auto cardObj = db()->getCard(cardId);
    return cardObj;
}

int TestServer::getPurchaseCount(unsigned int userId, unsigned int cardId) {
    auto result = db()->getUserCardData(userId, cardId);

    if (!result)
        return 0;

    return result->getPurchasesNumber();
}

bool TestServer::containsContact(const QByteArray &userId, const QByteArray &childId) {
    return db()->getContactFromChildId(userId, childId);
}

int TestServer::getFreeItemsCount(unsigned int userId, unsigned int cardId) {
    auto dbUsersCards = db()->getUserCardData(
        userId,
        cardId);
    return db()->getFreeItemsCount(dbUsersCards);
}


const QSharedPointer<RC::Interfaces::iDB> &TestServer::getDBObject() const {
    return db();
}
