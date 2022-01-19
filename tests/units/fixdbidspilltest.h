//#
//# Copyright (C) 2020-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef FIXDBIDSPILLTEST_H
#define FIXDBIDSPILLTEST_H

#include "test.h"
#include "testutils.h"



class FixDBIdsPillTest: public Test, protected TestUtils
{
public:
    FixDBIdsPillTest();

    void test() override;
};

#endif // FIXDBIDSPILLTEST_H
