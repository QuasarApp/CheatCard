//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "restoredatarequest.h"
namespace RC {

RestoreDataRequest::RestoreDataRequest() {

}

unsigned int RestoreDataRequest::userId() const {
    return _userId;
}

void RestoreDataRequest::setUserId(unsigned int newUserId) {
    _userId = newUserId;
}

bool RestoreDataRequest::isValid() const {
    return _userId;
}

QDataStream &RestoreDataRequest::fromStream(QDataStream &stream) {
    stream >> _userId;
    return stream;
}

QDataStream &RestoreDataRequest::toStream(QDataStream &stream) const {
    stream << _userId;

    return stream;

}

}
