//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef TESTCLIENT_H
#define TESTCLIENT_H

#include <CheatCard/client.h>

class TestClient: public RC::Client
{
    Q_OBJECT
public:
    TestClient(const QSharedPointer<RC::Interfaces::iDB> &db);
    void dropDB();
    int getPurchaseCount(const QByteArray& userId, const QByteArray& cardId);
    int getFreeItemsCount(const QByteArray& userId, const QByteArray& cardId);
    QSharedPointer<RC::Interfaces::iCard> getCard(const QByteArray& cardId) const;
    unsigned char getLastErrrorCode();
    const QSharedPointer<RC::Interfaces::iDB>& getDBObject() const;
    QSharedPointer<RC::Interfaces::iUser> getUser(const QByteArray& userId) const;

private slots:
    void handleRequestError(unsigned char code, QString msg);
private:

    unsigned char lastErrrorCode = 0;
};

#endif // TESTCLIENT_H
