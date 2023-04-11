//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "upgradedatabasetest.h"
#include "cheatcardtestshelper.h"

#include <testserver.h>

#define TEST_CHEAT_PORT 15004
#define TEST_CHEAT_HOST "localhost"

UpgradeDataBaseTest::UpgradeDataBaseTest()
{

}

void UpgradeDataBaseTest::test(){
    auto server = CheatCardTestsHelper::makeNode<TestServer>({3}, ":/sql/sql/DBv6.db.sql");
    QVERIFY(server);

    QVERIFY(server->run(TEST_CHEAT_HOST, TEST_CHEAT_PORT));
    server->stop();

}
