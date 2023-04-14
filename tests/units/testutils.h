//#
//# Copyright (C) 2020-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <QMetaObject>
#include <functional>

class TestUtils
{
public:
    TestUtils();
    static bool wait(const std::function<bool()> &forWait, int msec);


    static bool funcPrivateConnect(const std::function<bool ()> &requestFunc,
                            const std::function<bool ()> &checkFunc,
                            const std::function<QMetaObject::Connection ()> &connectFunction);

    static bool funcPrivateConnect(const std::function<bool ()> &requestFunc,
                            const std::function<bool ()> &checkFunc);

};

#endif // TESTUTILS_H
