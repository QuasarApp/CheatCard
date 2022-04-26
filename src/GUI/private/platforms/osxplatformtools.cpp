#include "osxplatformtools.h"
#include <QString>

namespace RC {

void OSXPlatformTools::setScreanDim(bool ) const {

}

bool OSXPlatformTools::getAccessToWriteInStorage() {
    return true;
}

bool OSXPlatformTools::getAccessToReadInStorage() {
    return true;
}

OSXPlatformTools::OSXPlatformTools() {

}

QString OSXPlatformTools::storeLink() const {
    return "https://apps.apple.com/app/id1603813109";
}
}
