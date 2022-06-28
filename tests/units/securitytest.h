#ifndef SECURITYTEST_H
#define SECURITYTEST_H

#include "test.h"
#include "testutils.h"

#include <QtTest>


class TestSeller;
class TestVisitor;
class TestServer;

/**
 * @brief The SecurityTest class  check security bugs and vulnerabilities
 */
class SecurityTest: public Test, protected TestUtils
{
public:
    SecurityTest();
    ~SecurityTest();
    void test();

private:
    void secureTest(const QSharedPointer<TestSeller> &seller,
                    const QSharedPointer<TestVisitor> &client,
                    const QSharedPointer<TestServer> &server);

};

#endif // SECURITYTEST_H
