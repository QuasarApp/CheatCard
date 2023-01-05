//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SECURITYTEST_H
#define SECURITYTEST_H

#include "connectiontest.h"

/**
 * @brief The SecurityTest class  check security bugs and vulnerabilities
 */
class SecurityTest: public ConnectionTest
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
