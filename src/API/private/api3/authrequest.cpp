
//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "authrequest.h"

namespace RC {
namespace API {
namespace V3 {

AuthRequest::AuthRequest() {

}

bool AuthRequest::isValid() const {
    return _userId.size() == 32;
}

const QByteArray& AuthRequest::userId() const {
    return _userId;
}

void AuthRequest::setUserId(const QByteArray &newUserId) {
    _userId = newUserId;
}

QDataStream &AuthRequest::fromStream(QDataStream &stream) {
    stream >> _userId;
    return stream;
}

QDataStream &AuthRequest::toStream(QDataStream &stream) const {
    stream << _userId;
    return stream;
}

}
}
}
