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
    _key = QCryptographicHash::hash(randomArray(), QCryptographicHash::Sha256);
    setId(QVariant::fromValue(qHash(_key)));
}

QH::PKG::DBObject *User::createDBObject() const {
    return new User();
}

QH::PKG::DBVariantMap User::variantMap() const {
    return {{"id",          {getId(),     QH::PKG::MemberType::PrimaryKey}},
            {"name",        {_name,       QH::PKG::MemberType::InsertUpdate}},
            {"key",         {_key,        QH::PKG::MemberType::InsertUpdate}},
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

     return stream;
}

QDataStream &User::toStream(QDataStream &stream) const {
    DBObject::toStream(stream);

    stream << _name;
    stream << _key;

    return stream;
}

QByteArray User::randomArray() const {

    QByteArray result;
    for (int length = 64 ;length >= 0 ; length--) {
        result.push_back(rand());
    }

    return result;
}

const QByteArray &User::getKey() const {
    return _key;
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
    setKey(q.value("key").toByteArray());

    return true;
}

}
