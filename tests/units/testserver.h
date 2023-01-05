//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <CheatCard/server.h>

class TestServer: public RC::Server
{
    Q_OBJECT
public:
    TestServer(const QSharedPointer<RC::Interfaces::iDB> &db);
    QSharedPointer<RC::Interfaces::iCard> getCard(unsigned int cardId) const;
    int getPurchaseCount(unsigned int userId,unsigned int cardId);
    int getFreeItemsCount(unsigned int userId, unsigned int cardId);
    bool containsContact(const QByteArray &userId, const QByteArray &childId);
    const QSharedPointer<RC::Interfaces::iDB>& getDBObject() const;

};

#endif // TESTSERVER_H
