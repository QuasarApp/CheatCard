//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef WINDOWSDESKTOPPLATFORMTOOL_H
#define WINDOWSDESKTOPPLATFORMTOOL_H
#include <iplatformtools.h>

#ifdef Q_OS_WINDOWS

namespace RC {

class WindowsDesktopPlatformTool: public IPlatformTools
{
public:
    WindowsDesktopPlatformTool();

    // IPlatformTools interface
public:
    void setScreanDim(bool enable) const override;
    bool getAccessToWriteInStorage() override;
    bool getAccessToReadInStorage() override;
};
}
#endif
#endif // WINDOWSDESKTOPPLATFORMTOOL_H
