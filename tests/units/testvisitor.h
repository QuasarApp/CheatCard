//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef TESTVISITOR_H
#define TESTVISITOR_H

#include <CheatCard/visitor.h>

class TestVisitor:  public RC::Visitor
{
public:
    TestVisitor(const QSharedPointer<RC::Interfaces::iDB> &db);
    QSharedPointer<RC::Interfaces::iCard> getCard(unsigned int cardId) const;
    int getPurchaseCount(unsigned int userId,unsigned int cardId);
    const QSharedPointer<RC::Interfaces::iDB>& getDBObject() const;
    int getFreeItemsCount(unsigned int userId, unsigned int cardId);
    void dropDB();
};

#endif // TESTVISITOR_H
