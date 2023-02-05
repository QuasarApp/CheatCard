//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "card.h"
#include <ctime>
#include <rci/rcutils.h>

namespace RC {
namespace API {
namespace V3 {

Card::Card() {
    setId(RCUtils::randomSHA256());
}

Card::Card(const QSharedPointer<Interfaces::iCard> &obj) {
    Card::fromObject(obj);
}

QDataStream &Card::fromStream(QDataStream &stream) {
    stream << _id;
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
    stream >> _ownerSignature;

    return stream;
}

QDataStream &Card::toStream(QDataStream &stream) const {
    stream << _id;
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
    stream << _ownerSignature;

    return stream;
}

const QByteArray &Card::id() const {
    return _id;
}

void Card::setId(const QByteArray &id) {
    _id = id;
}

QString Card::toString() const {
    QString result("id: %0 \n"
                   "title: %1 \n"
                   "phone: %2 \n"
                   "telegram: %3 \n"
                   "instagram: %4 \n"
                   "physicalAddress: %5 \n "
                   "webSite: %6 \n "
                   "freeItemName: %7 \n "
                   "freeIndex: %8 \n "
                   "cardVersion: %9 \n ");

    result = result.arg(QString(_id.toBase64(QByteArray::Base64UrlEncoding))).
            arg(_title,
                _phone,
                _telegramm,
                _instagramm,
                _physicalAddress,
                _webSite,
                _freeItemName).
            arg(freeIndex).
            arg(cardVersion);

    result += ("ownerId: %0 \n");
    result = result.arg(QString(_ownerSignature.toBase64(QByteArray::Base64UrlEncoding)));

    return result;
}

bool Card::isValid() const {
    return _id.size() == 32 && _ownerSignature.size() == 32;
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

QSharedPointer<Interfaces::iCard>
Card::toObject(const QSharedPointer<Interfaces::iDB> &db) const {
    if (!db)
        return nullptr;

    QSharedPointer<Interfaces::iCard> result = db->makeEmptyCard();
    result->setCardId(_id);
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
    result->setOwnerSignature(_ownerSignature);

    return result;
}

void Card::fromObject(const QSharedPointer<Interfaces::iCard> &obj) {
    if (obj) {
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
        _ownerSignature = obj->ownerSignature();
    }
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
