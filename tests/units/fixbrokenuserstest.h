#ifndef FIXBROKENUSERSTEST_H
#define FIXBROKENUSERSTEST_H
#include "test.h"
#include "testutils.h"


class FixBrokenUsersTest: public Test, protected TestUtils
{
public:
    FixBrokenUsersTest();
    void test() override;

};

#endif // FIXBROKENUSERSTEST_H
