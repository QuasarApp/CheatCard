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

const QString &Saller::getName() const {
    return name;
}

void Saller::setName(const QString &newName) {
    name = newName;
}

int Saller::getCardID() const {
    return cardID;
}

void Saller::setCardID(int newCardID) {
    cardID = newCardID;
}

QH::PKG::DBObject *Saller::createDBObject() const {
    return new Saller();
}

QString Saller::primaryKey() const {
    return "id";
}

QH::PKG::DBVariantMap Saller::variantMap() const {
    return
        {{"id",   {getId(), QH::PKG::MemberType::PrimaryKeyAutoIncrement}},
        {"name",  {name,    QH::PKG::MemberType::Insert}},
        {"image", {cardID,  QH::PKG::MemberType::InsertUpdate}},
    };
}

}
