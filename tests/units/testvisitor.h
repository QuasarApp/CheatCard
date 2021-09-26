#ifndef TESTVISITOR_H
#define TESTVISITOR_H

#include <CheatCard/visitor.h>

class TestDataBaseWrapper;


class TestVisitor:  public RC::Visitor
{
public:
    TestVisitor(QSharedPointer<TestDataBaseWrapper> db);
private:
    QSharedPointer<TestDataBaseWrapper> privateDb;
};

#endif // TESTVISITOR_H
