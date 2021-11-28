#ifndef TESTSELLER_H
#define TESTSELLER_H

#include <CheatCard/seller.h>


class TestDataBaseWrapper;

class TestSeller: public RC::Seller
{
    Q_OBJECT
public:
    TestSeller(QSharedPointer<TestDataBaseWrapper> db);
    void dropDB();
private:
    QSharedPointer<TestDataBaseWrapper> privateDb;
};

#endif // TESTSELLER_H
