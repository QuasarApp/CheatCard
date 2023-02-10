
//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "subscribetouserchanges.h"

namespace RC {
namespace API {
namespace V3 {

SubscribeToUserChanges::SubscribeToUserChanges() {

}

bool SubscribeToUserChanges::isValid() const {
    return _userId.size() == 32;
}

const QByteArray& SubscribeToUserChanges::userId() const {
    return _userId;
}

void SubscribeToUserChanges::setUserId(const QByteArray &newUserId) {
    _userId = newUserId;
}

QDataStream &SubscribeToUserChanges::fromStream(QDataStream &stream) {
    stream >> _userId;
    return stream;
}

QDataStream &SubscribeToUserChanges::toStream(QDataStream &stream) const {
    stream << _userId;
    return stream;
}

}
}
}
