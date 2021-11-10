#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <CheatCard/server.h>

class TestDataBaseWrapper;

class TestServer: public RC::ServerV1
{
    Q_OBJECT
public:
    TestServer(QSharedPointer<TestDataBaseWrapper>);
    QSharedPointer<RC::Card> getCard(unsigned int cardId) const;
    int getPurchaseCount(unsigned int userId,unsigned int cardId);

private:
    QSharedPointer<TestDataBaseWrapper> privateDb;
};

#endif // TESTSERVER_H
