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

    stream >> _minimum;
    stream >> _maximum;

    return stream;
}

QDataStream &ApplicationVersion::toStream(QDataStream &stream) const {
    stream << _minimum;
    stream << _maximum;

    return stream;
}

short ApplicationVersion::maximum() const {
    return _maximum;
}

void ApplicationVersion::setMaximum(short newMaximum) {
    _maximum = newMaximum;
}

short ApplicationVersion::minimum() const {
    return _minimum;
}

void ApplicationVersion::setMinimum(short newMinimum) {
    _minimum = newMinimum;
}

}
