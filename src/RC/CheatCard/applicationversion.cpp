//#
//# Copyright (C) 2020-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "applicationversion.h"

namespace RC {

ApplicationVersion::ApplicationVersion() {

}

QDataStream &ApplicationVersion::fromStream(QDataStream &stream) {

    stream >> _version;
    return stream;
}

QDataStream &ApplicationVersion::toStream(QDataStream &stream) const {
    stream << _version;
    return stream;
}

int ApplicationVersion::version() const {
    return _version;
}

void ApplicationVersion::setVersion(int newVersion) {
    _version = newVersion;
}

}
