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

bool RestoreDataRequest::isValid() const {
    return _userKey.size();
}

QDataStream &RestoreDataRequest::fromStream(QDataStream &stream) {
    stream >> _userKey;
    return stream;
}

QDataStream &RestoreDataRequest::toStream(QDataStream &stream) const {
    stream << _userKey;

    return stream;
}

const QByteArray &RestoreDataRequest::userKey() const {
    return _userKey;
}

QString RestoreDataRequest::userKeyBase64() const {
    return userKey().toBase64(QByteArray::Base64UrlEncoding);
}

void RestoreDataRequest::setUserKey(const QByteArray &newUserKey) {
    _userKey = newUserKey;
}

}
