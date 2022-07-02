#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <CheatCard/serverssl.h>

class TestDataBaseWrapper;

class TestServer: public RC::Server
{
    Q_OBJECT
public:
    TestServer(QSharedPointer<TestDataBaseWrapper>);
    QSharedPointer<RC::API::Card> getCard(unsigned int cardId) const;
    int getPurchaseCount(unsigned int userId,unsigned int cardId);
    bool containsContact(unsigned int userId, unsigned int childId);

private:
    QSharedPointer<TestDataBaseWrapper> privateDb;
};

#endif // TESTSERVER_H
