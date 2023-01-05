//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "card.h"
#include <ctime>

namespace RC {
namespace API {
namespace V0 {


Card::Card(): QH::PKG::AbstractData() {
    idGen();
}

Card::Card(const QSharedPointer<Interfaces::iCard> &obj) {
    _id = obj->cardId();
    _title = obj->title();
    _logo = obj->logo();
    _seal = obj->seal();
    _background = obj->background();
    _phone = obj->phone();
    _telegramm = obj->telegramm();
    _instagramm = obj->instagramm();
    _physicalAddress = obj->physicalAddress();
    _webSite = obj->webSite();
    freeIndex = obj->getFreeIndex();
    color = obj->getColor();
    fontColor = obj->getFontColor();
    _freeItemName = obj->freeItemName();
    cardVersion = obj->getCardVersion();
}

void Card::idGen() {
    setId(rand() + time(0));
}

bool Card::isValid() const {
    return _id;
}

const QByteArray &Card::logo() const {
    return _logo;
}

void Card::setLogo(const QByteArray &newImage) {
    _logo = newImage;
}

int Card::getFreeIndex() const {
    return freeIndex;
}

void Card::setFreeIndex(int newFreeIndex) {
    freeIndex = newFreeIndex;
}

const QByteArray &Card::ownerSignature() const {
    return _ownerSignature;
}

void Card::setOwnerSignature(const QByteArray &newOwnerSignature) {
    _ownerSignature = newOwnerSignature;
}

unsigned int Card::id() const {
    return _id;
}

void Card::setId(unsigned int newId) {
    _id = newId;
}

QSharedPointer<Interfaces::iCard> Card::toObject(const QSharedPointer<Interfaces::iDB> &db){
    if (!db)
        return nullptr;

    QSharedPointer<Interfaces::iCard> result = db->makeEmptyCard();
    result->setId(_id);
    result->setTitle(_title);
    result->setLogo(_logo);
    result->setSeal(_seal);
    result->setBackground(_background);
    result->setPhone(_phone);
    result->setTelegramm(_telegramm);
    result->setInstagramm(_instagramm);
    result->setPhysicalAddress(_physicalAddress);
    result->setWebSite(_webSite);
    result->setFreeIndex(freeIndex);
    result->setColor(color);
    result->setFontColor(fontColor);
    result->setFreeItemName(_freeItemName);
    result->setCardVersion(cardVersion);

    return result;
}

QDataStream &Card::fromStream(QDataStream &stream) {
    QVariant id;
    stream >> id;
    _id = id.toUInt();

    stream >> _title;
    stream >> _logo;
    stream >> _seal;
    stream >> _background;
    stream >> _phone;
    stream >> _telegramm;
    stream >> _instagramm;
    stream >> _physicalAddress;
    stream >> _webSite;
    stream >> freeIndex;
    stream >> color;
    stream >> fontColor;
    stream >> _freeItemName;
    stream >> cardVersion;

    return stream;
}

QDataStream &Card::toStream(QDataStream &stream) const {
    QVariant id(_id);
    stream << id;

    stream << _title;
    stream << _logo;
    stream << _seal;
    stream << _background;
    stream << _phone;
    stream << _telegramm;
    stream << _instagramm;
    stream << _physicalAddress;
    stream << _webSite;
    stream << freeIndex;
    stream << color;
    stream << fontColor;
    stream << _freeItemName;
    stream << cardVersion;

    return stream;
}

unsigned int Card::getCardVersion() const {
    return cardVersion;
}

void Card::setCardVersion(unsigned int newCardVersion) {
    cardVersion = newCardVersion;
}

const QString &Card::freeItemName() const {
    return _freeItemName;
}

void Card::setFreeItemName(const QString &newFreeItemName) {
    _freeItemName = newFreeItemName;
}

const QByteArray &Card::background() const {
    return _background;
}

void Card::setBackground(const QByteArray &newBackground) {
    _background = newBackground;
}

unsigned int Card::cardId() const {
    return id();
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

const QString &Card::getFontColor() const {
    return fontColor;
}

void Card::setFontColor(const QString &newColor) {
    fontColor = newColor;
}
}
}
}
