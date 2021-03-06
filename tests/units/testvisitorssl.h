#ifndef TESTVISITORSSL_H
#define TESTVISITORSSL_H

#include <CheatCard/visitorssl.h>

class TestDataBaseWrapper;


class TestVisitorSSL:  public RC::VisitorSSL
{
public:
    TestVisitorSSL(QSharedPointer<TestDataBaseWrapper> db);
    QSharedPointer<RC::API::Card> getCard(unsigned int cardId) const;
    int getPurchaseCount(unsigned int userId,unsigned int cardId);
private:
    QSharedPointer<TestDataBaseWrapper> privateDb;
};

#endif // TESTVISITORSSL_H
