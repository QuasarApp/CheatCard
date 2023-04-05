#include "testclient.h"

#include <CheatCard/clearolddata.h>

TestClient::TestClient(const QSharedPointer<RC::Interfaces::iDB> &db): RC::Client(db)  {
    connect(this, &Client::requestError, this, &TestClient::handleRequestError);
    connect(this, &Client::sigSyncReceived, this, &TestClient::handleSyncReceived);
}

void TestClient::dropDB() {

    // run force clear
    auto task = QSharedPointer<RC::ClearOldData>::create(db(), -1);
    task->setTime(0);
    task->setMode(QH::ScheduleMode::SingleWork);
    sheduleTask(task);
}

int TestClient::getPurchaseCount(const QByteArray &userId, const QByteArray &cardId) {
    auto result = db()->getUserCardData(userId, cardId);

    if (!result)
        return 0;

    return result->getPurchasesNumber();
}

QSharedPointer<RC::Interfaces::iContacts> TestClient::getContact(const QByteArray &parentUser,
                                                                 const QByteArray &childUser) {
    return db()->getContactFromChildId(parentUser, childUser);
}

int TestClient::getFreeItemsCount(const QByteArray &userId, const QByteArray &cardId) {
    auto dbUsersCards = db()->getUserCardData(
        userId,
        cardId);
    return db()->getFreeItemsCount(dbUsersCards);
}

QSharedPointer<RC::Interfaces::iCard> TestClient::getCard(const QByteArray &cardId) const {
    return db()->getCard(cardId);
}

QSharedPointer<RC::Interfaces::iUsersCards> TestClient::getUserData(const QByteArray &userId,
                                                                    const QByteArray &cardId) const {
    return db()->getUserCardData(userId, cardId);
}

unsigned char TestClient::getLastErrrorCode() {
    unsigned char lastErr = lastErrrorCode;
    lastErrrorCode = 0;
    return lastErr;
}

void TestClient::resetLastErrors() {
    getLastErrrorCode();
}

void TestClient::handleSyncReceived() {
    _synced = true;
}

bool TestClient::isSynced() const {
    return _synced;
}

const QSharedPointer<RC::Interfaces::iDB> &TestClient::getDBObject() const {
    return db();
}

QSharedPointer<RC::Interfaces::iUser> TestClient::getUser(const QByteArray &userId) const {
    return db()->getUser(userId);
}

QSharedPointer<RC::Interfaces::iUser> TestClient::getCurrentUser() const {
    return getUser(currntUserKey());
}

void TestClient::handleRequestError(unsigned char code, QString ) {
    lastErrrorCode = code;

}
