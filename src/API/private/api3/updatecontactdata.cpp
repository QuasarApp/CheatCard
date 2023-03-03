//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "updatecontactdata.h"



namespace RC {
namespace API {
namespace V3 {

UpdateContactData::UpdateContactData() {

}

QDataStream &UpdateContactData::fromStream(QDataStream &stream) {
    stream >> _contacts;
    stream >> _userSecreet;
    stream >> remove;
    return stream;
}

QDataStream &UpdateContactData::toStream(QDataStream &stream) const {
    stream << _contacts;
    stream << _userSecreet;
    stream << remove;

    return stream;
}

QH::PKG::DataPack<API::V3::Contacts> UpdateContactData::contacts() const {
    return _contacts;
}

void UpdateContactData::setContacts(const QH::PKG::DataPack<API::V3::Contacts> &newContacts) {
    _contacts = newContacts;
}

void UpdateContactData::addContact(const QSharedPointer<Contacts> &newContacts) {
    _contacts.push(newContacts);
}

bool UpdateContactData::getRemove() const {
    return remove;
}

void UpdateContactData::setRemove(bool newRemove) {
    remove = newRemove;
}

bool UpdateContactData::isValid() const {
    return _contacts.isValid() && _userSecreet.size();
}

const QByteArray &UpdateContactData::userSecreet() const {
    return _userSecreet;
}

void UpdateContactData::setUserSecreet(const QByteArray &newUserSecreet) {
    _userSecreet = newUserSecreet;
}

}
}
}
