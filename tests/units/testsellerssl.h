#ifndef TESTSELLERSSL_H
#define TESTSELLERSSL_H

#include <CheatCard/sellerssl.h>

class TestDataBaseWrapper;

class TestSellerSSL: public RC::SellerSSL
{
    Q_OBJECT
public:
    TestSellerSSL(QSharedPointer<TestDataBaseWrapper> db);
private:
    QSharedPointer<TestDataBaseWrapper> privateDb;

};

#endif // TESTSELLER_H
