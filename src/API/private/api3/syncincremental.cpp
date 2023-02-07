
//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "syncincremental.h"


namespace RC {
namespace API {
namespace V3 {


SyncIncremental::SyncIncremental(){}


bool SyncIncremental::isValid() const {
    return _syncedUserKey.size() == 32;
}

QDataStream &SyncIncremental::fromStream(QDataStream &stream) {
    stream << _usersCardsToAdd;
    stream << _usersCardsToRemove;
    stream << _contactsToRemove;
    stream << _contactsToAdd;
    stream << _syncedUserKey;

    return stream;
}

QDataStream &SyncIncremental::toStream(QDataStream &stream) const {

    stream << _usersCardsToAdd;
    stream << _usersCardsToRemove;
    stream << _contactsToRemove;
    stream << _contactsToAdd;
    stream << _syncedUserKey;

    return stream;
}

QH::PKG::DataPack<API::V3::UsersCards> SyncIncremental::usersCardsToRemove() const {
    return _usersCardsToRemove;
}

void SyncIncremental::setUsersCardsToRemove(const QH::PKG::DataPack<API::V3::UsersCards> &newUsersCardsToRemove) {
    _usersCardsToRemove = newUsersCardsToRemove;
}

QH::PKG::DataPack<API::V3::Contacts> SyncIncremental::contactsToRemove() const {
    return _contactsToRemove;
}

void SyncIncremental::setContactsToRemove(const QH::PKG::DataPack<API::V3::Contacts> &newContactsToRemove) {
    _contactsToRemove = newContactsToRemove;
}

QH::PKG::DataPack<API::V3::Contacts> SyncIncremental::contactsToAdd() const {
    return _contactsToAdd;
}

void SyncIncremental::setContactsToAdd(const QH::PKG::DataPack<API::V3::Contacts> &newContactsToAdd) {
    _contactsToAdd = newContactsToAdd;
}

const QByteArray& SyncIncremental::syncedUserKey() const {
    return _syncedUserKey;
}

void SyncIncremental::setSyncedUserKey(const QByteArray &newSyncedUserKey) {
    _syncedUserKey = newSyncedUserKey;
}

QH::PKG::DataPack<API::V3::UsersCards> SyncIncremental::usersCardsToAdd() const {
    return _usersCardsToAdd;
}

void SyncIncremental::setUsersCardsToAdd(const QH::PKG::DataPack<API::V3::UsersCards> &newUsersCardsToAdd) {
    _usersCardsToAdd = newUsersCardsToAdd;
}

}
}
}
