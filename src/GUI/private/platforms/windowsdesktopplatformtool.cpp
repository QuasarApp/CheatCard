//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "windowsdesktopplatformtool.h"
#include <QString>

#ifdef Q_OS_WINDOWS

namespace RC {

WindowsDesktopPlatformTool::WindowsDesktopPlatformTool()
{

}

void RC::WindowsDesktopPlatformTool::setScreanDim(bool ) const {
    // to-do
}

bool WindowsDesktopPlatformTool::getAccessToWriteInStorage() {
    // do nothing
    return true;

}

bool WindowsDesktopPlatformTool::getAccessToReadInStorage() {
    // do nothing
    return true;

}

QString WindowsDesktopPlatformTool::storeLink() const {
    return "https://www.cheatcard.net/";
}

}
#endif
