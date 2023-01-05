
//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "contacts.h"
#include "rci/rcutils.h"

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
    return {{"userKey",      {QString(userKey.toBase64(QByteArray::Base64UrlEncoding)),      QH::PKG::MemberType::Insert}},
            {"childUserKey", {QString(childUserKey.toBase64(QByteArray::Base64UrlEncoding)), QH::PKG::MemberType::Insert}},
            {"info",         {info,         QH::PKG::MemberType::InsertUpdate}},

            };
}

bool Contacts::isValid() const {
    return userKey.size() && childUserKey.size();
}

QString Contacts::primaryKey() const {
    return "";
}

QString Contacts::primaryValue() const {
    return QString(userKey.toBase64(QByteArray::Base64UrlEncoding));
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

QString Contacts::condition() const {
    QString strUserKey(userKey.toBase64(QByteArray::Base64UrlEncoding));
    QString strChildUserKey(childUserKey.toBase64(QByteArray::Base64UrlEncoding));

    return QString("userKey='%0' AND childUserKey='%1'").
        arg(strUserKey, strChildUserKey);
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

bool Contacts::fromSqlRecord(const QSqlRecord &q) {

    childUserKey = QByteArray::fromBase64(q.value("childUserKey").toByteArray(),
                                          QByteArray::Base64UrlEncoding);
    userKey = QByteArray::fromBase64(q.value("userKey").toByteArray(),
                                     QByteArray::Base64UrlEncoding);
    info = q.value("info").toString();

    return true;
}

}
}
