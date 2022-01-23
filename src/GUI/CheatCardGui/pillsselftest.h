//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef PILLSSELFTEST_H
#define PILLSSELFTEST_H

#include <isqldbcache.h>

namespace RC {

class PillsSelfTest
{
public:
    PillsSelfTest();
    static bool testBadCatrdId(QH::ISqlDBCache *db);
};


}
#endif // PILLSSELFTEST_H
