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
            {"name",        {_name,      QH::PKG::MemberType::Insert}},
            {"card",        {_cardID,    QH::PKG::MemberType::InsertUpdate}}};
}

QString User::primaryKey() const {
    return "id";
}

int User::cardID() const {
    return _cardID;
}

void User::setCardID(int newCardID) {
    _cardID = newCardID;
}

const QString &User::name() const {
    return _name;
}

void User::setName(const QString &newName) {
    _name = newName;
}
}
