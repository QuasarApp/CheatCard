
//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "contacts.h"
#include "user.h"

namespace RC {
namespace API {

Contacts::Contacts(): QH::PKG::DBObject("Contacts") {}

QH::PKG::DBObject *Contacts::createDBObject() const {
    return new Contacts();
}

QH::PKG::DBVariantMap Contacts::variantMap() const {
    return {{"userKey",      {QString(userKey.toBase64(QByteArray::Base64UrlEncoding)),      QH::PKG::MemberType::Insert}},
            {"childUserKey", {QString(childUserKey.toBase64(QByteArray::Base64UrlEncoding)), QH::PKG::MemberType::Insert}},
            {"genesisKey",   {genesisKey,   QH::PKG::MemberType::Insert}},
            {"info",         {info,         QH::PKG::MemberType::InsertUpdate}},

    };
}

bool Contacts::isValid() const {
    return userKey.size() && childUserKey.size() && genesisKey;
}

QString Contacts::primaryKey() const {
    return "";
}

QDataStream &Contacts::fromStream(QDataStream &stream) {

    stream >> userKey;
    stream >> childUserKey;
    stream >> genesisKey;
    stream >> info;

    return stream;
}

QDataStream &Contacts::toStream(QDataStream &stream) const {

    stream << userKey;
    stream << childUserKey;
    stream << genesisKey;
    stream << info;

    return stream;
}

QString Contacts::condition() const {
    QString strUserKey(userKey.toBase64(QByteArray::Base64UrlEncoding));
    return QString("userKey='%0' AND genesisKey='%1'").arg(strUserKey).arg(genesisKey);
}

const QByteArray &Contacts::getUserKey() const {
    return userKey;
}

void Contacts::setUserKey(const QByteArray &newUserKey) {
    userKey = newUserKey;
}

QSharedPointer<User> Contacts::toUser(const QSharedPointer<API::User>& currentUser) const {
    auto result = QSharedPointer<User>::create();

    auto inputData = currentUser->secret();
    auto currentUserKey = API::User::makeKey(inputData);

    if (currentUserKey != userKey) {
        return {};
    }

    inputData.insert(inputData.size(), reinterpret_cast<const char*>(&genesisKey), sizeof (genesisKey));
    result->setSecret(QCryptographicHash::hash(inputData, QCryptographicHash::Sha256));
    result->regenerateKeys();
    result->setName(info);

    return result;
}

const QByteArray &Contacts::getChildUserKey() const {
    return childUserKey;
}

void Contacts::setChildUserKey(const QByteArray &newChildUserKey) {
    childUserKey = newChildUserKey;
}

int Contacts::getGenesisKey() const {
    return genesisKey;
}

void Contacts::setGenesisKey(int newGenesisKey) {
    genesisKey = newGenesisKey;
}

unsigned int Contacts::getChildUserId() const {
    return API::User::makeId(childUserKey);
}

unsigned int Contacts::getUser() const {
    return API::User::makeId(userKey);
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
    genesisKey = q.value("genesisKey").toInt();

    userKey = QByteArray::fromBase64(q.value("userKey").toByteArray(),
                                    QByteArray::Base64UrlEncoding);
    info = q.value("info").toUInt();

    return true;
}

}
}
