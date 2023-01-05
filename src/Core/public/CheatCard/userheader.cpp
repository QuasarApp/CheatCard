//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "userheader.h"

#include <rci/objects/iuser.h>
namespace RC {

UserHeader::UserHeader() {

}

bool UserHeader::isValid() const {
    return _token.size() == 32 && _userId > 0 && _sessionId && AbstractData::isValid();;
}

QDataStream &UserHeader::fromStream(QDataStream &stream) {

    stream >> _token;
    stream >> _sessionId;
    stream >> _userId;
    stream >> _userName;

    return stream;
}

QDataStream &UserHeader::toStream(QDataStream &stream) const {

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

bool UserHeader::toUser(QSharedPointer<Interfaces::iUser> &out) const {

    if (!isValid()) {
        return false;
    }

    out->setKey(token());
    out->setId(getUserId());
    out->setName(userName());
    out->setSecret("");

    return true;
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

