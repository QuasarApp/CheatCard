//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "sync.h"
#include "api3/userscards.h"
#include <api3/contacts.h>


namespace RC {
namespace API {
namespace V3 {

Sync::Sync() {

}

bool Sync::isValid() const {
    return _userKey.size() == 32;
}

QDataStream &Sync::fromStream(QDataStream &stream) {
    stream >> _usersCards;
    stream >> _contacts;
    stream >> _userKey;

    return stream;
}

QDataStream &Sync::toStream(QDataStream &stream) const {
    stream << _usersCards;
    stream << _contacts;
    stream << _userKey;

    return stream;
}

const QByteArray &Sync::userKey() const {
    return _userKey;
}

void Sync::setUserKey(const QByteArray &newUserKey) {
    _userKey = newUserKey;
}

const QH::PKG::DataPack<API::V3::Contacts> &Sync::contacts() const {
    return _contacts;
}

void Sync::setContacts(const QH::PKG::DataPack<API::V3::Contacts> &newContacts) {
    _contacts = newContacts;
}

const QH::PKG::DataPack<API::V3::UsersCards> &Sync::usersCards() const {
    return _usersCards;
}

void Sync::setUsersCards(const QH::PKG::DataPack<API::V3::UsersCards> &newUsersCards) {
    _usersCards = newUsersCards;
}

}
}
}
