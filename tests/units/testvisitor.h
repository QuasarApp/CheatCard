#ifndef TESTVISITOR_H
#define TESTVISITOR_H

#include <CheatCard/visitor.h>

class TestDataBaseWrapper;


class TestVisitor:  public RC::Visitor
{
public:
    TestVisitor(QSharedPointer<TestDataBaseWrapper> db);
    QSharedPointer<RC::API::Card> getCard(unsigned int cardId) const;
    int getPurchaseCount(unsigned int userId,unsigned int cardId);
    void dropDB();
private:
    QSharedPointer<TestDataBaseWrapper> privateDb;
};

#endif // TESTVISITOR_H
