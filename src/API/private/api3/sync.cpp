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
    // mode is not be Restrict and Incremental on one time
    return _mode && _syncedUserKey.size() == 32;
}

QDataStream &Sync::fromStream(QDataStream &stream) {
    stream >> _usersCards;
    stream >> _contacts;
    stream >> _mode;
    stream >> _syncedUserKey;

    return stream;
}

QDataStream &Sync::toStream(QDataStream &stream) const {
    stream << _usersCards;
    stream << _contacts;
    stream << _mode;
    stream << _syncedUserKey;

    return stream;
}

QByteArray Sync::syncedUserKey() const {
    return _syncedUserKey;
}

void Sync::setSyncedUserKey(const QByteArray &newSyncedUserKey) {
    _syncedUserKey = newSyncedUserKey;
}

Sync::Mode Sync::mode() const {
    return _mode;
}

void Sync::setMode(Mode newMode) {
    _mode = newMode;
}

bool Sync::isContainsPermisionsInfo() const {
    return _mode & Mode::Contacts;
}

bool Sync::isContainsUsersDataInfo() const {
    return _mode & Mode::UsersData;
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
