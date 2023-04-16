//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "user.h"
#include "QCryptographicHash"
#include "qaglobalutils.h"
#include <rci/rcutils.h>

namespace RC {
namespace DBv0 {

User::User() {
}

void User::regenerateKeys(const QByteArray &newSecret) {
    if (newSecret.size() == 32) {
        _secret = newSecret;
    } else {
        _secret = QCryptographicHash::hash(randomArray(), QCryptographicHash::Sha256);
    }

    _key = RC::RCUtils::makeUserKey(_secret);
}

QH::PKG::DBObject *User::createDBObject() const {
    return new User();
}

QH::PKG::DBVariantMap User::variantMap() const {
    return {{"id",          {_id,     QH::PKG::MemberType::PrimaryKey}},
            {"name",        {_name,       QH::PKG::MemberType::InsertUpdate}},
            {"key",         {QString(_key.toBase64(QByteArray::Base64UrlEncoding)),    QH::PKG::MemberType::InsertUpdate}},
            {"secret",      {QString(_secret.toBase64(QByteArray::Base64UrlEncoding)), QH::PKG::MemberType::InsertUpdate}},
            {"time",        {static_cast<int>(time(0)),      QH::PKG::MemberType::InsertUpdate}},
    };
}

bool User::isValid() const {

    if (_secret.size() && _secret.size() != 32)
        return false;

    return DBObject::isValid() && _key.size();
}

bool User::isAllKeysIsValid() const {
    if (_secret.size()) {
        return RC::RCUtils::makeUserKey(_secret) == _key && _key.size();
    }

    return _key.size();
}

QString User::primaryKey() const {
    return "id";
}

QVariant User::primaryValue() const {
    return _id;
}

QDataStream &User::fromStream(QDataStream &stream) {
    QVariant id;

    stream >> id;
    _id = id.toUInt();
    stream >> _name;
    stream >> _key;
    stream >> _secret;
    stream >> _fSaller;

    return stream;
}

QDataStream &User::toStream(QDataStream &stream) const {
    QVariant id(_id);

    stream << id;

    stream << _name;
    stream << _key;
    stream << _secret;
    stream << _fSaller;

    return stream;
}

QString User::table() const {
    return "Users";
}

QByteArray User::randomArray() const {

    QByteArray result;
    ::randomArray(64, result);
    return result;
}

unsigned int User::id() const {
    return _id;
}

void User::setId(unsigned int newId) {
    _id = newId;
}

QString User::toString() const {
    QString result("id: %0 \n"
                   "name: %1 \n"
                   "publicKey: %2 \n"
                   "secreetKey: %3 \n"
                   "seller: %4 \n");

    result = result.arg(id()).
             arg(name()).
             arg(getKey(),
                 secret()).
             arg(fSaller());

    return result;
}

const QByteArray &User::secret() const {
    return _secret;
}

void User::setSecret(const QByteArray &newSecret) {
    _secret = newSecret;
}

const QByteArray &User::getKey() const {
    return _key;
}

const QString User::getSignature() const {
    return getKey().toBase64(QByteArray::Base64UrlEncoding);
}

void User::setKey(const QByteArray &newKey){
    _key = newKey;
}

bool User::fSaller() const {
    return _fSaller;
}

void User::setFSaller(bool newFSaller) {
    _fSaller = newFSaller;

    if (secret().isEmpty()) {
        regenerateKeys();
    }
}

const QString &User::name() const {
    return _name;
}

void User::setName(const QString &newName) {
    _name = newName;
}

bool User::fromSqlRecord(const QSqlRecord &q) {

    _id = q.value("id").toUInt();
    setName(q.value("name").toString());
    setKey(QByteArray::fromBase64(q.value("key").toByteArray(),
                                  QByteArray::Base64UrlEncoding));
    setSecret(QByteArray::fromBase64(q.value("secret").toByteArray(),
                                     QByteArray::Base64UrlEncoding));

    return true;
}

}
}
