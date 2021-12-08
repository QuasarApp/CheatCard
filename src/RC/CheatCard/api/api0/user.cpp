//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "user.h"
#include "QCryptographicHash"
namespace RC {

User::User(): QH::PKG::DBObject("Users") {
    regenerateKeys();
    srand(time(0));
}

void RC::User::regenerateKeys() {
    _secret = QCryptographicHash::hash(randomArray(), QCryptographicHash::Sha256);
    _key = makeKey(_secret);
    setId(QVariant::fromValue(makeId(_key)));
}

QH::PKG::DBObject *User::createDBObject() const {
    return new User();
}

QH::PKG::DBVariantMap User::variantMap() const {
    return {{"id",          {getId(),     QH::PKG::MemberType::PrimaryKey}},
            {"name",        {_name,       QH::PKG::MemberType::InsertUpdate}},
            {"key",         {QString(_key.toBase64(QByteArray::Base64UrlEncoding)),    QH::PKG::MemberType::Insert}},
            {"secret",      {QString(_secret.toBase64(QByteArray::Base64UrlEncoding)), QH::PKG::MemberType::Insert}},
            {"time",        {static_cast<int>(time(0)),      QH::PKG::MemberType::InsertUpdate}},

    };
}

QString User::primaryKey() const {
    return "id";
}

QDataStream &User::fromStream(QDataStream &stream) {
     DBObject::fromStream(stream);

     stream >> _name;
     stream >> _key;
     stream >> _secret;
     stream >> _fSaller;

     return stream;
}

QDataStream &User::toStream(QDataStream &stream) const {
    DBObject::toStream(stream);

    stream << _name;
    stream << _key;
    stream << _secret;
    stream << _fSaller;

    return stream;
}

QByteArray User::randomArray() const {

    QByteArray result;
    for (int length = 64 ;length >= 0 ; length--) {
        result.push_back(rand());
    }

    return result;
}

const QByteArray &User::secret() const {
    return _secret;
}

void User::setSecret(const QByteArray &newSecret) {
    _secret = newSecret;
}

QByteArray User::makeKey(const QByteArray &secret) {
    return QCryptographicHash::hash(secret, QCryptographicHash::Sha256);
}

unsigned int User::makeId(const QByteArray &key) {
    return qHash(key);
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

unsigned int User::userId() const {
    return getId().toUInt();
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

    setId(q.value("id").toUInt());
    setName(q.value("name").toString());
    setKey(QByteArray::fromBase64(q.value("key").toByteArray(),
                                  QByteArray::Base64UrlEncoding));
    setSecret(QByteArray::fromBase64(q.value("secret").toByteArray(),
                                     QByteArray::Base64UrlEncoding));

    return true;
}

}
