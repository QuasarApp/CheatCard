//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "userheader.h"
namespace RC {

UserHeader::UserHeader() {

}

bool UserHeader::isValid() const {
    return _token.size() == 32 && userId > 0 && sessionId && AbstractData::isValid();;
}

QDataStream &UserHeader::fromStream(QDataStream &stream) {
    AbstractData::fromStream(stream);

    stream >> _token;
    stream >> sessionId;
    stream >> userId;

    return stream;
}

QDataStream &UserHeader::toStream(QDataStream &stream) const {
    AbstractData::toStream(stream);

    stream << _token;
    stream << sessionId;
    stream << userId;

    return stream;
}

const QByteArray &UserHeader::token() const {
    return _token;
}

void UserHeader::setToken(const QByteArray &newToken) {
    _token = newToken;
}

long long UserHeader::getSessionId() const {
    return sessionId;
}

void UserHeader::setSessionId(long long newSessionId) {
    sessionId = newSessionId;
}

unsigned int UserHeader::getUserId() const {
    return userId;
}

void UserHeader::setUserId(unsigned int newUserId) {
    userId = newUserId;
}
}
