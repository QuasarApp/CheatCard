//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "userheader.h"
namespace RC {

UserHeader::UserHeader() {

}

bool UserHeader::isValid() const {
    return _token.size() == 32 && _userId > 0 && _sessionId && AbstractData::isValid();;
}

QDataStream &UserHeader::fromStream(QDataStream &stream) {
    AbstractData::fromStream(stream);

    stream >> _token;
    stream >> _sessionId;
    stream >> _userId;
    stream >> _userName;

    return stream;
}

QDataStream &UserHeader::toStream(QDataStream &stream) const {
    AbstractData::toStream(stream);

    stream << _token;
    stream << _sessionId;
    stream << _userId;
    stream << _userName;

    return stream;
}

const QString &UserHeader::userName() const {
    return _userName;
}

void UserHeader::setUserName(const QString &newUserName) {
    _userName = newUserName;
}

const QByteArray &UserHeader::token() const {
    return _token;
}

void UserHeader::setToken(const QByteArray &newToken) {
    _token = newToken;
}

long long UserHeader::getSessionId() const {
    return _sessionId;
}

void UserHeader::setSessionId(long long newSessionId) {
    _sessionId = newSessionId;
}

unsigned int UserHeader::getUserId() const {
    return _userId;
}

void UserHeader::setUserId(unsigned int newUserId) {
    _userId = newUserId;
}
}
