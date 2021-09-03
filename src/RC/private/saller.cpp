//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "saller.h"

namespace RC {

Saller::Saller(): QH::PKG::DBObject("Sallers") {

}

QH::PKG::DBObject *Saller::createDBObject() const {
    return new Saller();
}

QString Saller::primaryKey() const {
    return "id";
}

const QString &Saller::getName() const {
    return name;
}

void Saller::setName(const QString &newName) {
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

bool Saller::fromSqlRecord(const QSqlRecord &q) {
    if (!DBObject::fromSqlRecord(q)) {
        return false;
    }

    setName(q.value("name").toString());

    return true;
}

QH::PKG::DBVariantMap Saller::variantMap() const {
    return
        {{"id",   {getId(), QH::PKG::MemberType::PrimaryKeyAutoIncrement}},
        {"name",  {name,    QH::PKG::MemberType::PrimaryKey}}
    };
}

}
