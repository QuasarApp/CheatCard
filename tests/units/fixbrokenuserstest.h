//#
//# Copyright (C) 2020-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


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
