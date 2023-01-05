//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "restoreresponce.h"
#include "api2/userscards.h"
#include <api0/contacts.h>


namespace RC {
namespace API {
namespace V2 {

RestoreResponce::RestoreResponce() {

}

bool RestoreResponce::isValid() const {
    return _userKey.size();
}

QDataStream &RestoreResponce::fromStream(QDataStream &stream) {
    stream >> _usersCards;
    stream >> _contacts;
    stream >> _userKey;

    return stream;
}

QDataStream &RestoreResponce::toStream(QDataStream &stream) const {
    stream << _usersCards;
    stream << _contacts;
    stream << _userKey;

    return stream;
}

const QByteArray &RestoreResponce::userKey() const {
    return _userKey;
}

void RestoreResponce::setUserKey(const QByteArray &newUserKey) {
    _userKey = newUserKey;
}

const QH::PKG::DataPack<API::V0::Contacts> &RestoreResponce::contacts() const {
    return _contacts;
}

void RestoreResponce::setContacts(const QH::PKG::DataPack<API::V0::Contacts> &newContacts) {
    _contacts = newContacts;
}

const QH::PKG::DataPack<API::V2::UsersCards> &RestoreResponce::usersCards() const {
    return _usersCards;
}

void RestoreResponce::setUsersCards(const QH::PKG::DataPack<API::V2::UsersCards> &newUsersCards) {
    _usersCards = newUsersCards;
}

}
}
}
