#include "testserver.h"

#include <QCoreApplication>

TestServer::TestServer(const QSharedPointer<RC::Interfaces::iDB> &db,
                       const QVector<unsigned short> &apiVesions):
    RC::Server(db, apiVesions) {
}

QSharedPointer<RC::Interfaces::iCard> TestServer::getCard(const QByteArray& cardId) const {

    auto cardObj = db()->getCard(cardId);
    return cardObj;
}

int TestServer::getPurchaseCount(const QByteArray& userId, const QByteArray& cardId) {
    auto result = db()->getUserCardData(userId, cardId);

    if (!result)
        return 0;

    return result->getPurchasesNumber();
}

bool TestServer::containsContact(const QByteArray &userId, const QByteArray &childId) {
    return db()->getContactFromChildId(userId, childId);
}

int TestServer::getFreeItemsCount(const QByteArray& userId, const QByteArray& cardId) {
    auto dbUsersCards = db()->getUserCardData(
        userId,
        cardId);
    return db()->getFreeItemsCount(dbUsersCards);
}


const QSharedPointer<RC::Interfaces::iDB> &TestServer::getDBObject() const {
    return db();
}

void TestServer::TestServer::nodeConnected(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConnected(node);
}
