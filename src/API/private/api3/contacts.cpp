//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "contacts.h"

namespace RC {
namespace API {
namespace V3 {

Contacts::Contacts(): QH::PKG::AbstractData() {}

Contacts::Contacts(const QSharedPointer<Interfaces::iContacts> &contacts) {
    Contacts::fromObject(contacts);
}

bool Contacts::isValid() const {
    return userKey.size() && childUserKey.size();
}

QDataStream &Contacts::fromStream(QDataStream &stream) {

    stream >> userKey;
    stream >> childUserKey;
    stream >> info;

    return stream;
}

QDataStream &Contacts::toStream(QDataStream &stream) const {

    stream << userKey;
    stream << childUserKey;
    stream << info;

    return stream;
}

const QByteArray &Contacts::getUserKey() const {
    return userKey;
}

void Contacts::setUserKey(const QByteArray &newUserKey) {
    userKey = newUserKey;
}

QSharedPointer<Interfaces::iContacts>
Contacts::toObject(const QSharedPointer<Interfaces::iDB> &db) const {
    if (!db)
        return nullptr;

    QSharedPointer<Interfaces::iContacts> result = db->makeEmptyContact();

    result->setChildUserKey(childUserKey);
    result->setUserKey(userKey);
    result->setInfo(info);

    return result;
}

void Contacts::fromObject(const QSharedPointer<Interfaces::iContacts> &obj) {
    userKey = obj->getUserKey();
    childUserKey = obj->getChildUserKey();
    info = obj->getInfo();
}

const QByteArray &Contacts::getChildUserKey() const {
    return childUserKey;
}

void Contacts::setChildUserKey(const QByteArray &newChildUserKey) {
    childUserKey = newChildUserKey;
}

const QString &Contacts::getInfo() const {
    return info;
}

void Contacts::setInfo(const QString &newInfo) {
    info = newInfo;
}

}
}
}
