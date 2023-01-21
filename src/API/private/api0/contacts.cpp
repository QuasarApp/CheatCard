//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "contacts.h"
#include "rci/rcutils.h"

namespace RC {
namespace API {
namespace V0 {

Contacts::Contacts(): QH::PKG::AbstractData() {}

Contacts::Contacts(const QSharedPointer<Interfaces::iContacts> &contacts) {
    userKey = contacts->getUserKey();
    childUserKey = contacts->getChildUserKey();
    info = contacts->getInfo();
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
Contacts::toObject(const QSharedPointer<Interfaces::iDB> &db) {
    if (!db)
        return nullptr;

    QSharedPointer<Interfaces::iContacts> result = db->makeEmptyContact();

    result->setChildUserKey(childUserKey);
    result->setUserKey(userKey);
    result->setInfo(info);

    return result;
}

const QByteArray &Contacts::getChildUserKey() const {
    return childUserKey;
}

void Contacts::setChildUserKey(const QByteArray &newChildUserKey) {
    childUserKey = newChildUserKey;
}

unsigned int Contacts::getChildUserId() const {
    return RC::RCUtils::makeUserId(childUserKey);
}

unsigned int Contacts::getUser() const {
    return RC::RCUtils::makeUserId(userKey);
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
