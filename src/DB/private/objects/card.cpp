//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "card.h"
#include <ctime>
#include "qaglobalutils.h"

namespace RC {
namespace DB {



Card::Card() {
    Card::idGen();
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
    _ownerSignature = obj->ownerSignature();
}

QH::PKG::DBObject *Card::createDBObject() const {
    return new Card();
}

QDataStream &Card::fromStream(QDataStream &stream) {
    stream >> _id;
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

    result = result.
            arg(_id.toBase64(QByteArray::Base64UrlEncoding),
                _title,
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

void Card::idGen() {
    _id.clear();
    randomArray(32, _id);
}

QH::PKG::DBVariantMap Card::variantMap() const {
    return {{"id",              {_id,             QH::PKG::MemberType::PrimaryKey}},
        {"title",           {_title,          QH::PKG::MemberType::InsertUpdate}},
        {"logo",            {_logo,           QH::PKG::MemberType::InsertUpdate}},
        {"seal",            {_seal,           QH::PKG::MemberType::InsertUpdate}},
        {"background",      {_background,     QH::PKG::MemberType::InsertUpdate}},
        {"color",           {color,           QH::PKG::MemberType::InsertUpdate}},
        {"fontColor",       {fontColor,       QH::PKG::MemberType::InsertUpdate}},

        {"phone",           {_phone,          QH::PKG::MemberType::InsertUpdate}},
        {"telegramm",       {_telegramm,      QH::PKG::MemberType::InsertUpdate}},
        {"instagramm",      {_instagramm,     QH::PKG::MemberType::InsertUpdate}},
        {"physicalAddress", {_physicalAddress,QH::PKG::MemberType::InsertUpdate}},
        {"webSite",         {_webSite,        QH::PKG::MemberType::InsertUpdate}},
        {"freeItemName",    {_freeItemName,   QH::PKG::MemberType::InsertUpdate}},

        {"freeIndex",       {freeIndex,       QH::PKG::MemberType::InsertUpdate}},
        {"time",            {static_cast<int>(time(0)),      QH::PKG::MemberType::InsertUpdate}},
        {"cardVersion",     {cardVersion,     QH::PKG::MemberType::InsertUpdate}},
        {"ownerSignature",  {QString(_ownerSignature.toBase64(QByteArray::Base64UrlEncoding)), QH::PKG::MemberType::InsertUpdate}},

    };
}

bool Card::compare(const QSharedPointer<iCard> &other) {
    auto obj = other.dynamicCast<Card>();
    if (!obj)
        return false;

    return QH::PKG::DBObject::compare(*obj);
}

bool Card::isValid() const {
    return _id.size() && _ownerSignature.size() && _freeItemName.size() && freeIndex > 0;
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

bool Card::fromSqlRecord(const QSqlRecord &q) {

    setCardId(QByteArray::fromBase64(q.value("id").toByteArray(),
                                 QByteArray::Base64UrlEncoding));
    setTitle(q.value("title").toString());
    setLogo(q.value("logo").toByteArray());
    setSeal(q.value("seal").toByteArray());
    setBackground(q.value("background").toByteArray());

    setPhone(q.value("phone").toByteArray());
    setTelegramm(q.value("telegramm").toByteArray());
    setInstagramm(q.value("instagramm").toByteArray());
    setPhysicalAddress(q.value("physicalAddress").toByteArray());
    setWebSite(q.value("webSite").toByteArray());
    setColor(q.value("color").toString());
    setFontColor(q.value("fontColor").toString());

    setFreeIndex(q.value("freeIndex").toInt());
    setFreeItemName(q.value("freeItemName").toString());

    setCardVersion(q.value("cardVersion").toUInt());

    setOwnerSignature(QByteArray::fromBase64(q.value("ownerSignature").toByteArray(),
                                             QByteArray::Base64UrlEncoding));

    return true;
}

QString Card::table() const {
    return "Cards";
}

QString Card::primaryKey() const {
    return "id";
}

QString Card::primaryValue() const {
    return _id.toBase64(QByteArray::Base64UrlEncoding);
}

const QByteArray &Card::ownerSignature() const {
    return _ownerSignature;
}

void Card::setOwnerSignature(const QByteArray &newOwnerSignature) {
    _ownerSignature = newOwnerSignature;
}

bool Card::isOvner(const QByteArray& userId) const {
    return _ownerSignature == userId;
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

const QByteArray& Card::cardId() const {
    return _id;
}

void Card::setCardId(const QByteArray &newId) {
    _id = newId;
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
