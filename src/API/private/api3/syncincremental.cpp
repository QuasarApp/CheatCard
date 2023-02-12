
//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "contacts.h"
#include "syncincremental.h"
#include "userscards.h"


namespace RC {
namespace API {
namespace V3 {


SyncIncremental::SyncIncremental(){}


bool SyncIncremental::isValid() const {
    return true;
}

QDataStream &SyncIncremental::fromStream(QDataStream &stream) {
    stream << _usersCardsToAdd;
    stream << _usersCardsToRemove;
    stream << _contactsToRemove;
    stream << _contactsToAdd;
    stream << result;

    return stream;
}

QDataStream &SyncIncremental::toStream(QDataStream &stream) const {

    stream << _usersCardsToAdd;
    stream << _usersCardsToRemove;
    stream << _contactsToRemove;
    stream << _contactsToAdd;
    stream << result;

    return stream;
}

bool SyncIncremental::getResult() const {
    return result;
}

void SyncIncremental::setResult(bool newResult) {
    result = newResult;
}

const QH::PKG::DataPack<API::V3::UsersCards>& SyncIncremental::usersCardsToRemove() const {
    return _usersCardsToRemove;
}

void SyncIncremental::setUsersCardsToRemove(const QH::PKG::DataPack<API::V3::UsersCards> &newUsersCardsToRemove) {
    _usersCardsToRemove = newUsersCardsToRemove;
}

const QH::PKG::DataPack<API::V3::Contacts>& SyncIncremental::contactsToRemove() const {
    return _contactsToRemove;
}

void SyncIncremental::setContactsToRemove(const QH::PKG::DataPack<API::V3::Contacts> &newContactsToRemove) {
    _contactsToRemove = newContactsToRemove;
}

void SyncIncremental::addContactsToRemove(const QSharedPointer<Contacts> &newContactsToRemove) {
    _contactsToRemove.push(newContactsToRemove);
}

const QH::PKG::DataPack<API::V3::Contacts>& SyncIncremental::contactsToAdd() const {
    return _contactsToAdd;
}

void SyncIncremental::setContactsToAdd(const QH::PKG::DataPack<API::V3::Contacts> &newContactsToAdd) {
    _contactsToAdd = newContactsToAdd;
}

void SyncIncremental::addContactsToAdd(const QSharedPointer<Contacts> &newContactsToAdd) {
    _contactsToAdd.push(newContactsToAdd);
}

const QH::PKG::DataPack<API::V3::UsersCards>& SyncIncremental::usersCardsToAdd() const {
    return _usersCardsToAdd;
}

void SyncIncremental::setUsersCardsToAdd(const QH::PKG::DataPack<API::V3::UsersCards> &newUsersCardsToAdd) {
    _usersCardsToAdd = newUsersCardsToAdd;
}

void SyncIncremental::addUsersCardsToAdd(const QSharedPointer<UsersCards> &userData) {
    _usersCardsToAdd.push(userData);
}

}
}
}
