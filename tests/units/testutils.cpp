//#
//# Copyright (C) 2020-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "testutils.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QVariantMap>
#include <config.h>

bool TestUtils::funcPrivateConnect(const std::function<bool()> &requestFunc,
                                   const std::function<bool()> &checkFunc,
                                   const std::function<QMetaObject::Connection()> &connectFunction) {

    QMetaObject::Connection m_connection = connectFunction();
    if (requestFunc && !requestFunc()) {
        QObject::disconnect(m_connection);
        return false;
    }

    bool return_value = TestUtils::wait(checkFunc, WAIT_RESPOCE_TIME);
    QObject::disconnect(m_connection);

    return return_value;
}

bool TestUtils::funcPrivateConnect(const std::function<bool ()> &requestFunc,
                                   const std::function<bool ()> &checkFunc) {
    return funcPrivateConnect(requestFunc, checkFunc, [](){return QMetaObject::Connection();});
}

TestUtils::TestUtils()
{

}

bool TestUtils::wait(const std::function<bool()> &forWait, int msec) {
    auto curmsec = QDateTime::currentMSecsSinceEpoch() + msec;
    bool condition = forWait();
    while (curmsec > QDateTime::currentMSecsSinceEpoch() && !condition) {
        QCoreApplication::processEvents();
        condition = forWait();
    }
    QCoreApplication::processEvents();
    return condition;
}

