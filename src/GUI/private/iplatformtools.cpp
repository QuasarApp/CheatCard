//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "iplatformtools.h"

#include <platforms/androidplatformtools.h>
#include <platforms/linuxdesktopplatfomtools.h>
#include <platforms/windowsdesktopplatformtool.h>
#include <platforms/osxplatformtools.h>
#include <platforms/iosplatformtools.h>


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

#ifdef Q_OS_MACOS
    static IPlatformTools * instance = new OSXPlatformTools();
#endif

#ifdef Q_OS_IOS
    static IPlatformTools * instance = new IOSPlatformTools();
#endif

    return instance;
}

}
