//#
//# Copyright (C) 2020-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef VERSIONISRECEIVED_H
#define VERSIONISRECEIVED_H

#include <abstractdata.h>

namespace RC {

class VersionIsReceived: public QH::PKG::AbstractData
{
    QH_PACKAGE(VersionIsReceived, "VersionIsReceived")

public:
    VersionIsReceived();
};

}
#endif // VERSIONISRECEIVED_H
