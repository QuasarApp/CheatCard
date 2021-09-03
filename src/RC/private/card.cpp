//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "card.h"

namespace RC {

Card::Card(): QH::PKG::DBObject("Cards") {

}

QH::PKG::DBObject *Card::createDBObject() const {
    return new Card();
}

QH::PKG::DBVariantMap Card::variantMap() const {
    return {{"id",              {getId(),         QH::PKG::MemberType::PrimaryKeyAutoIncrement}},
            {"name",            {_name,           QH::PKG::MemberType::PrimaryKey}},
            {"image",           {_image,          QH::PKG::MemberType::InsertUpdate}},
            {"purchasesNumber", {purchasesNumber, QH::PKG::MemberType::InsertUpdate}},
            {"freeIndex",       {freeIndex,       QH::PKG::MemberType::InsertUpdate}}};
}

const QString &Card::name() const {
    return _name;
}

void Card::setName(const QString &newName) {
    _name = newName;
}

const QByteArray &Card::image() const {
    return _image;
}

void Card::setImage(const QByteArray &newImage) {
    _image = newImage;
}

int Card::getPurchasesNumber() const {
    return purchasesNumber;
}

void Card::setPurchasesNumber(int newPurchasesNumber) {
    purchasesNumber = newPurchasesNumber;
}

int Card::getFreeIndex() const {
    return freeIndex;
}

void Card::setFreeIndex(int newFreeIndex) {
    freeIndex = newFreeIndex;
}

bool Card::fromSqlRecord(const QSqlRecord &q) {
    if (!DBObject::fromSqlRecord(q)) {
        return false;
    }

    setName(q.value("name").toString());
    setImage(q.value("image").toByteArray());
    setPurchasesNumber(q.value("purchasesNumber").toInt());
    setFreeIndex(q.value("freeIndex").toInt());

    return true;
}

QString Card::primaryKey() const {
    return "id";
}
}
