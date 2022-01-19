//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "pillsselftest.h"
#include "doctorpill.h"
#include "pills/invalidcardidpill.h"

namespace RC {


PillsSelfTest::PillsSelfTest() {
}

bool PillsSelfTest::testBadCatrdId(QH::ISqlDBCache *db) const {
    DP::DoctorTest test;

    if (!db)
        return false;

    return test.test({QSharedPointer<InvalidCardIdPill>::create(db)}, true);
}

}
