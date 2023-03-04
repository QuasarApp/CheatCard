//#
//# Copyright (C) 2023-2023 QuasarApp.
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
namespace V3 {

User::User(): QH::PKG::AbstractData() {
    regenerateKeys();
}

void User::regenerateKeys() {
    _secret = QCryptographicHash::hash(randomArray(), QCryptographicHash::Sha256);
    _key = RC::RCUtils::makeUserKey(_secret);
}

bool User::isValid() const {
    return _key.size();
}

bool User::isAllKeysIsValid() const {
    if (_secret.size()) {
        return RC::RCUtils::makeUserKey(_secret) == _key && _key.size() == 32;
    }

    return _key.size() == 32;
}

QDataStream &User::fromStream(QDataStream &stream) {

    stream >> _name;
    stream >> _key;
    stream >> _secret;
    stream >> _fSaller;

    return stream;
}

QDataStream &User::toStream(QDataStream &stream) const {

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

const QByteArray &User::secret() const {
    return _secret;
}

void User::setSecret(const QByteArray &newSecret) {
    _secret = newSecret;
}

const QByteArray &User::getKey() const {
    return _key;
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

}
}
}
