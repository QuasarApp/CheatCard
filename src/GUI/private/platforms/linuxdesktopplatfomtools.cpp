//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "linuxdesktopplatfomtools.h"
#include <QString>

namespace RC {

LinuxDesktopPlatfomTools::LinuxDesktopPlatfomTools()
{

}

void LinuxDesktopPlatfomTools::setScreanDim(bool ) const {
    // TO-DO
}

bool LinuxDesktopPlatfomTools::getAccessToWriteInStorage() {
    // do nothing
    return true;
}

bool LinuxDesktopPlatfomTools::getAccessToReadInStorage() {
    // do nothing
    return true;

}

QString LinuxDesktopPlatfomTools::storeLink() const {
    return "https://www.cheatcard.net/";
}
}
