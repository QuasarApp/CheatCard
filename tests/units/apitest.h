//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef APITEST_H
#define APITEST_H

#include "test.h"

#include <QtTest>

/**
 * @brief The APITest class this test check process functions of the api.
 */
class APITest: public Test
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
