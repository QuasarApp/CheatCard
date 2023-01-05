//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "user.h"
#include "QCryptographicHash"
#include "qaglobalutils.h"
#include "rci/rcutils.h"

namespace RC {
namespace API {
namespace V0 {

User::User(): QH::PKG::AbstractData() {
    regenerateKeys();
}

void User::regenerateKeys() {
    _secret = QCryptographicHash::hash(randomArray(), QCryptographicHash::Sha256);
    _key = RC::RCUtils::makeUserKey(_secret);
    setId(RC::RCUtils::makeUserId(_key));
}

bool User::isValid() const {
    return _key.size();
}

bool User::isAllKeysIsValid() const {
    if (_secret.size()) {
        return RC::RCUtils::makeUserKey(_secret) == _key && RC::RCUtils::makeUserId(_key) == userId();
    }

    return RC::RCUtils::makeUserId(_key) == userId();
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

unsigned int User::userId() const {
    return id();
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

}
}
}
