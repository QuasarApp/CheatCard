//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "user.h"
namespace RC {

User::User(): QH::PKG::DBObject("Users") {

}

QH::PKG::DBObject *User::createDBObject() const {
    return new User();
}

QH::PKG::DBVariantMap User::variantMap() const {
    return {{"id",          {getId(),    QH::PKG::MemberType::PrimaryKeyAutoIncrement}},
            {"name",        {_name,      QH::PKG::MemberType::PrimaryKey}}
    };
}

QString User::primaryKey() const {
    return "id";
}

const QString &User::name() const {
    return _name;
}

void User::setName(const QString &newName) {
    if (_name == newName)
        return;
    _name = newName;
    emit nameChanged();
}

bool User::fromSqlRecord(const QSqlRecord &q) {
    if (!DBObject::fromSqlRecord(q)) {
        return false;
    }

    setName(q.value("name").toString());

    return true;
}

}
