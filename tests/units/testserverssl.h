#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <CheatCard/serverssl.h>

class TestDataBaseWrapper;

class TestServerSSL: public RC::ServerSSL
{
    Q_OBJECT
public:
    TestServerSSL(QSharedPointer<TestDataBaseWrapper>);
    QSharedPointer<RC::Card> getCard(unsigned int cardId) const;
    int getPurchaseCount(unsigned int userId,unsigned int cardId);

private:
    QSharedPointer<TestDataBaseWrapper> privateDb;
};

#endif // TESTSERVER_H
