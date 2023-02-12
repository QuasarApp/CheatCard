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
    return _userKey.size() == 32 && AbstractData::isValid();;
}

QDataStream &UserHeader::fromStream(QDataStream &stream) {

    stream >> _userKey;
    stream >> _userName;

    return stream;
}

QDataStream &UserHeader::toStream(QDataStream &stream) const {
    stream << _userKey;
    stream << _userName;

    return stream;
}

const QByteArray& UserHeader::userKey() const {
    return _userKey;
}

void UserHeader::setUserKey(const QByteArray &newUserKey) {
    _userKey = newUserKey;
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

    out->setKey(_userKey);
    out->setName(userName());
    out->setSecret("");

    return true;
}
}

