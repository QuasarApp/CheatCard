#ifndef RESTOREDATATEST_H
#define RESTOREDATATEST_H


#include "test.h"
#include "testutils.h"

#include <QtTest>

#include <CheatCard/basenode.h>


class TestSeller;
class TestVisitor;
class TestServer;

class RestoreDataTest: public Test, protected TestUtils
{
public:
    RestoreDataTest();
    void test();

private:
    void restoreTest(const QSharedPointer<TestSeller> &seller,
                     const QSharedPointer<TestVisitor> &client,
                     const QSharedPointer<TestServer> &server);

};

#endif // RESTOREDATATEST_H
