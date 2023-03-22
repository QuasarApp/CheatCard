//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef UPGRADEDATABASETEST_H
#define UPGRADEDATABASETEST_H
#include "test.h"
#include <QtTest>

class UpgradeDataBaseTest: public Test
{
public:
    UpgradeDataBaseTest();
    void test() override;

};

#endif // UPGRADEDATABASETEST_H
