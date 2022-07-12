#ifndef APITEST_H
#define APITEST_H

#include "test.h"
#include "testutils.h"

#include <QtTest>

/**
 * @brief The APITest class this test check process functions of the api.
 */
class APITest: public Test, protected TestUtils
{
public:
    APITest();

    // Test interface
public:
    void test() override;
private:
    void testProcessDeleteCardRequest();
};

#endif // APITEST_H
