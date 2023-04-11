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
    TestServer(const QSharedPointer<RC::Interfaces::iDB> &db, const QVector<unsigned short> &apiVesions);
    QSharedPointer<RC::Interfaces::iCard> getCard(const QByteArray& cardId) const;
    int getPurchaseCount(const QByteArray& userId, const QByteArray& cardId);
    int getFreeItemsCount(const QByteArray& userId, const QByteArray& cardId);
    bool containsContact(const QByteArray &userId, const QByteArray &childId);
    const QSharedPointer<RC::Interfaces::iDB>& getDBObject() const;

    void nodeConnected(QH::AbstractNodeInfo *node) override;

};

#endif // TESTSERVER_H
