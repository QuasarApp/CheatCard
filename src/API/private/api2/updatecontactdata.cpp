//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "updatecontactdata.h"



namespace RC {
namespace API {
namespace V2 {

UpdateContactData::UpdateContactData() {

}

UpdateContactData::UpdateContactData(const Interfaces::iContacts &contact) {
    setInfo(contact.getInfo());
    setUserKey(contact.getUserKey());
    setChildUserKey(contact.getChildUserKey());
}

QDataStream &UpdateContactData::fromStream(QDataStream &stream) {
    API::V0::Contacts::fromStream(stream);

    stream >> _userSecreet;
    stream >> remove;
    return stream;
}

QDataStream &UpdateContactData::toStream(QDataStream &stream) const {
    API::V0::Contacts::toStream(stream);

    stream << _userSecreet;
    stream << remove;

    return stream;
}

bool UpdateContactData::getRemove() const {
    return remove;
}

void UpdateContactData::setRemove(bool newRemove) {
    remove = newRemove;
}

bool UpdateContactData::isValid() const {
    return API::V0::Contacts::isValid() && _userSecreet.size();
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
