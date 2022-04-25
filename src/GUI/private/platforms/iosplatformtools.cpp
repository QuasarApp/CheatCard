//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "iosplatformtools.h"
#include <QString>

namespace RC {

IOSPlatformTools::IOSPlatformTools()
{

}

void IOSPlatformTools::setScreanDim(bool ) const {
    // TO-DO
}

bool IOSPlatformTools::getAccessToWriteInStorage() {
    return true;
}

bool IOSPlatformTools::getAccessToReadInStorage() {
    return true;
}

QString IOSPlatformTools::storeLink() const {
    return "https://apps.apple.com/app/id1603813109";
}
}
