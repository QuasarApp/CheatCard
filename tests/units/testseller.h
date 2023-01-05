//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef TESTSELLER_H
#define TESTSELLER_H

#include <CheatCard/seller.h>


class TestSeller: public RC::Seller
{
    Q_OBJECT
public:
    TestSeller(const QSharedPointer<RC::Interfaces::iDB> &db);
    void dropDB();
    int getPurchaseCount(unsigned int userId, unsigned int cardId);
    int getFreeItemsCount(unsigned int userId, unsigned int cardId);
    QSharedPointer<RC::Interfaces::iCard> getCard(unsigned int cardId) const;
    unsigned char getLastErrrorCode();
    const QSharedPointer<RC::Interfaces::iDB>& getDBObject() const;
    QSharedPointer<RC::Interfaces::iUser> getUser(unsigned int userId) const;

private slots:
    void handleRequestError(unsigned char code, QString msg);
private:

    unsigned char lastErrrorCode = 0;
};

#endif // TESTSELLER_H
