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
            {"title",           {_title,          QH::PKG::MemberType::InsertUpdate}},
            {"logo",            {_logo,           QH::PKG::MemberType::InsertUpdate}},
            {"seal",            {_seal,           QH::PKG::MemberType::InsertUpdate}},
            {"background",      {_background,     QH::PKG::MemberType::InsertUpdate}},
            {"color",           {color,           QH::PKG::MemberType::InsertUpdate}},

            {"phone",           {_phone,          QH::PKG::MemberType::InsertUpdate}},
            {"telegramm",       {_telegramm,      QH::PKG::MemberType::InsertUpdate}},
            {"instagramm",      {_instagramm,     QH::PKG::MemberType::InsertUpdate}},
            {"physicalAddress", {_physicalAddress,QH::PKG::MemberType::InsertUpdate}},
            {"webSite",         {_webSite,        QH::PKG::MemberType::InsertUpdate}},

            {"purchasesNumber", {purchasesNumber, QH::PKG::MemberType::InsertUpdate}},
            {"freeIndex",       {freeIndex,       QH::PKG::MemberType::InsertUpdate}}};
}

bool Card::isValid() const {
    return _name.size();
}

const QString &Card::name() const {
    return _name;
}

void Card::setName(const QString &newName) {
    _name = newName;
}

const QByteArray &Card::logo() const {
    return _logo;
}

void Card::setLogo(const QByteArray &newImage) {
    _logo = newImage;
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


    setId(q.value("id").toInt());
    setName(q.value("name").toString());
    setTitle(q.value("title").toString());
    setLogo(q.value("logo").toByteArray());
    setSeal(q.value("seal").toByteArray());
    setSeal(q.value("background").toByteArray());

    setPhone(q.value("phone").toByteArray());
    setTelegramm(q.value("telegramm").toByteArray());
    setInstagramm(q.value("instagramm").toByteArray());
    setPhysicalAddress(q.value("physicalAddress").toByteArray());
    setWebSite(q.value("webSite").toByteArray());
    setColor(q.value("color").toString());

    setPurchasesNumber(q.value("purchasesNumber").toInt());
    setFreeIndex(q.value("freeIndex").toInt());

    return true;
}

QString Card::primaryKey() const {
    return "id";
}

const QByteArray &Card::background() const {
    return _background;
}

void Card::setBackground(const QByteArray &newBackground) {
    _background = newBackground;
}

const QString &Card::title() const {
    return _title;
}

void Card::setTitle(const QString &newTitle) {
    _title = newTitle;
}

const QString &Card::webSite() const {
    return _webSite;
}

void Card::setWebSite(const QString &newWebSite) {
    _webSite = newWebSite;
}

const QString &Card::physicalAddress() const {
    return _physicalAddress;
}

void Card::setPhysicalAddress(const QString &newPhysicalAddress) {
    _physicalAddress = newPhysicalAddress;
}

const QString &Card::instagramm() const {
    return _instagramm;
}

void Card::setInstagramm(const QString &newInstagramm) {
    _instagramm = newInstagramm;
}

const QString &Card::telegramm() const {
    return _telegramm;
}

void Card::setTelegramm(const QString &newTelegramm) {
    _telegramm = newTelegramm;
}

const QString &Card::phone() const {
    return _phone;
}

void Card::setPhone(const QString &newPhone) {
    _phone = newPhone;
}

const QByteArray &Card::seal() const {
    return _seal;
}

void Card::setSeal(const QByteArray &newSeal) {
    _seal = newSeal;
}

const QString &Card::getColor() const {
    return color;
}

void Card::setColor(const QString &newColor) {
    color = newColor;
}
}
