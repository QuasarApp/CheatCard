//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "user.h"
#include "QCryptographicHash"
namespace RC {

User::User(): QH::PKG::DBObject("Users") {
    _key = QCryptographicHash::hash(randomArray(), QCryptographicHash::Sha256);
}

QH::PKG::DBObject *User::createDBObject() const {
    return new User();
}

QH::PKG::DBVariantMap User::variantMap() const {
    return {{"id",          {getId(),     QH::PKG::MemberType::PrimaryKeyAutoIncrement}},
            {"name",        {_name,       QH::PKG::MemberType::PrimaryKey}},
            {"key",         {_key,        QH::PKG::MemberType::InsertUpdate}},
            {"visiblename", {_visibleName,QH::PKG::MemberType::InsertUpdate}},
            {"fSaller",     {_fSaller,    QH::PKG::MemberType::InsertUpdate}}

    };
}

QString User::primaryKey() const {
    return "id";
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

const QString &User::visibleName() const {
    return _visibleName;
}

void User::setVisibleName(const QString &newVisibleName) {
    _visibleName = newVisibleName;
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
    setVisibleName(newName);
}

bool User::fromSqlRecord(const QSqlRecord &q) {

    setId(q.value("id").toInt());
    setName(q.value("name").toString());
    setKey(q.value("key").toByteArray());
    setVisibleName(q.value("visiblename").toString());

    setFSaller(q.value("fSaller").toBool());

    return true;
}

}
