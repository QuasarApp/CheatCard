//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef RESTOREDATATEST_H
#define RESTOREDATATEST_H

#include "test.h"
#include <QtTest>


class TestClient;
class TestServer;

class RestoreDataTest: public Test
{
public:
    RestoreDataTest();
    void test();

};

#endif // RESTOREDATATEST_H
