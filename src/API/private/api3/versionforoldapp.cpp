//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "versionforoldapp.h"

namespace RC {
namespace API {
namespace V3 {

unsigned short VersionForOldApp::command(){
    return  30356; // qHash(QString("ApplicationVersion")) % 0xFFFF on Qt 5.12.2
}

QString VersionForOldApp::commandText(){return "VersionForOldApp";}
unsigned short VersionForOldApp::cmd() const {return VersionForOldApp::command();}
QString VersionForOldApp::cmdString() const {return VersionForOldApp::commandText();}
unsigned int VersionForOldApp::localCode() const {return typeid (*this).hash_code();}

VersionForOldApp::VersionForOldApp()
{

}

bool VersionForOldApp::isValid() const {
    return true;
}

bool VersionForOldApp::isOldPackage() const {
    return true;
}

QDataStream &VersionForOldApp::fromStream(QDataStream &stream) {
    return stream;
}

QDataStream &VersionForOldApp::toStream(QDataStream &stream) const {
    stream << static_cast<short>(3);
    stream << static_cast<short>(3);

    return stream;
}


}
}
}
