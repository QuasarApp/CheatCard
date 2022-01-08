//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef LINUXDESKTOPPLATFOMTOOLS_H
#define LINUXDESKTOPPLATFOMTOOLS_H
#include <iplatformtools.h>

namespace RC {

class LinuxDesktopPlatfomTools: public IPlatformTools
{

public:
    void setScreanDim(bool enable) const override;
    bool getAccessToWriteInStorage() override;
    bool getAccessToReadInStorage() override;

protected:
    LinuxDesktopPlatfomTools();

    // IPlatformTools interface

    friend class IPlatformTools;
};
}
#endif // LINUXDESKTOPPLATFOMTOOLS_H
