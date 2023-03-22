
//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "contacts.h"

namespace RC {
namespace DB {

Contacts::Contacts(): QH::PKG::DBObject() {}

Contacts::Contacts(const QSharedPointer<Interfaces::iContacts> &obj) {
    userKey = obj->getUserKey();
    childUserKey = obj->getChildUserKey();
    info = obj->getInfo();
}

QH::PKG::DBObject *Contacts::createDBObject() const {
    return new Contacts();
}

QString Contacts::table() const {
    return "Contacts";
}

QSharedPointer<Interfaces::iContacts> Contacts::toObject(const QSharedPointer<Interfaces::iDB> &db) {

    if (!db)
        return nullptr;

    QSharedPointer<Interfaces::iContacts> result = db->makeEmptyContact();

    result->setChildUserKey(childUserKey);
    result->setUserKey(userKey);
    result->setInfo(info);

    return result;
}

QH::PKG::DBVariantMap Contacts::variantMap() const {
    return {{"userKey",      {userKey,      QH::PKG::MemberType::Insert}},
            {"childUserKey", {childUserKey, QH::PKG::MemberType::Insert}},
            {"info",         {info,         QH::PKG::MemberType::InsertUpdate}},

            };
}

bool Contacts::isValid() const {
    return userKey.size() && childUserKey.size();
}

std::pair<QString, QMap<QString, QVariant> > Contacts::condition() const {

    return {QString("userKey=:userKey AND childUserKey=:childUserKey"),
            {{":userKey", userKey},
             {":childUserKey", childUserKey}}};
}

const QByteArray &Contacts::getUserKey() const {
    return userKey;
}

void Contacts::setUserKey(const QByteArray &newUserKey) {
    userKey = newUserKey;
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

bool Contacts::fromSqlRecord(const QSqlRecord &q) {

    childUserKey = q.value("childUserKey").toByteArray();
    userKey = q.value("userKey").toByteArray();
    info = q.value("info").toString();

    return true;
}

}
}
