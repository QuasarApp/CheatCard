//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "iplatformtools.h"

#include <platforms/androidplatformtools.h>
#include <platforms/linuxdesktopplatfomtools.h>
#include <platforms/windowsdesktopplatformtool.h>


namespace RC {

IPlatformTools *IPlatformTools::instance() {

#ifdef Q_OS_LINUX
#ifdef Q_OS_ANDROID
    static IPlatformTools * instance = new AndroidPlatformTools();
#else
    static IPlatformTools * instance = new LinuxDesktopPlatfomTools();
#endif
#endif

#ifdef Q_OS_WINDOWS
    static IPlatformTools * instance = new WindowsDesktopPlatformTool();
#endif

    return instance;
}

}
