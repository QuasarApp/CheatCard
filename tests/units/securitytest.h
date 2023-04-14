//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SECURITYTEST_H
#define SECURITYTEST_H

#include "test.h"

/**
 * @brief The SecurityTest class  check security bugs and vulnerabilities
 */
class SecurityTest: public Test
{
public:
    SecurityTest();
    ~SecurityTest();
    void test();


};

#endif // SECURITYTEST_H
