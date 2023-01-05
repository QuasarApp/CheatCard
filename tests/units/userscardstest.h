//#
//# Copyright (C) 2020-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef USERSCARDSTEST_H
#define USERSCARDSTEST_H

#include "test.h"
#include "testutils.h"

#include <QtTest>
class UsersCardsTest: public Test, protected TestUtils
{
public:
    UsersCardsTest();

    // Test interface
public:
    void test() override;
};

#endif // USERSCARDSTEST_H
