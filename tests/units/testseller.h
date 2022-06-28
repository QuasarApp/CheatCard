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
    int getPurchaseCount(unsigned int userId, unsigned int cardId);
    unsigned char getLastErrrorCode() const;

private slots:
    void handleRequestError(unsigned char code, QString msg);
private:

    unsigned char lastErrrorCode = 0;

    QSharedPointer<TestDataBaseWrapper> privateDb;
};

#endif // TESTSELLER_H
