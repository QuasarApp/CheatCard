//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "userscards.h"
#include <rci/rcutils.h>
#include <QDateTime>
#include <rci/core/idb.h>

namespace RC {
namespace DB {


UsersCards::UsersCards() {
}

UsersCards::UsersCards(const QByteArray& user, const QByteArray& card) {
    UsersCards::setUser(user);
    UsersCards::setCard(card);
}

UsersCards::UsersCards(const QSharedPointer<Interfaces::iUsersCards> &obj) {
    UsersCards::setUser(obj->getUser());
    UsersCards::setCard(obj->getCard());
    UsersCards::setPurchasesNumber(obj->getPurchasesNumber());
    UsersCards::setReceived(obj->getReceived());
    UsersCards::setCardVersion(obj->getCardVersion());
    UsersCards::setTime(obj->getRawTime());

}

QH::PKG::DBObject *UsersCards::createDBObject() const {
    return new UsersCards();
}

QH::PKG::DBVariantMap UsersCards::variantMap() const {
    return {{"user",           {user.toBase64(QByteArray::Base64UrlEncoding),            QH::PKG::MemberType::Insert}},
            {"card",           {card.toBase64(QByteArray::Base64UrlEncoding),            QH::PKG::MemberType::Insert}},
            {"purchasesNumber",{purchasesNumber, QH::PKG::MemberType::InsertUpdate}},
            {"received",       {received,        QH::PKG::MemberType::InsertUpdate}},
            {"time",           {_time,           QH::PKG::MemberType::InsertUpdate}},
            };
}

QString UsersCards::primaryKey() const {
    return "";
}

QString UsersCards::primaryValue() const {
    return "";
}

QString UsersCards::toString() const {
    QString result(
                   "user: %0 \n"
                   "card: %1 \n"
                   "purchasesNumber: %2 \n"
                   "received: %3 \n"
                   "time: %4 \n ");

    result = result.
             arg(user.toBase64(QByteArray::Base64UrlEncoding),
                 card.toBase64(QByteArray::Base64UrlEncoding)).
             arg(purchasesNumber).
             arg(received).
             arg(_time);

    return result;
}

QDataStream &UsersCards::fromStream(QDataStream &stream) {

    DBObject::fromStream(stream); // clazy:exclude=skipped-base-method

    stream >> user;
    stream >> card;
    stream >> purchasesNumber;
    stream >> received;
    stream >> cardVersion;
    stream >> _time;

    return stream;
}

QDataStream &UsersCards::toStream(QDataStream &stream) const {
    DBObject::toStream(stream); // clazy:exclude=skipped-base-method

    stream << user;
    stream << card;
    stream << purchasesNumber;
    stream << received;
    stream << cardVersion;
    stream << _time;

    return stream;
}

void UsersCards::setTime(unsigned int newTime) {
    _time = newTime;
}

bool UsersCards::isActive() const {
    return QDateTime::currentDateTime() <
           QDateTime::fromSecsSinceEpoch(_time).addSecs(RC::Interfaces::ACTIVE_USER_TIME_LIMIT);
}

unsigned int UsersCards::getCardVersion() const {
    return cardVersion;
}

void UsersCards::setCardVersion(unsigned int newCardVersion) {
    cardVersion = newCardVersion;
}

unsigned int UsersCards::getRawTime() const {
    return _time;
}

unsigned int UsersCards::getReceived() const {
    return received;
}

void UsersCards::setReceived(unsigned int newReceived) {
    received = newReceived;
}

void UsersCards::receive(unsigned int receiveFreItemsCount) {
    setReceived(received + receiveFreItemsCount);
}

unsigned int UsersCards::getPurchasesNumber() const {
    return purchasesNumber;
}

void UsersCards::setPurchasesNumber(unsigned int newPurchasesNumber) {
    purchasesNumber = newPurchasesNumber;
}

const QByteArray& UsersCards::getCard() const {
    return card;
}

void UsersCards::setCard(const QByteArray& newCard) {
    card = newCard;
}

const QByteArray& UsersCards::getUser() const {
    return user;
}

void UsersCards::setUser(const QByteArray& newUser) {
    user = newUser;
}

bool UsersCards::fromSqlRecord(const QSqlRecord &q) {

    user = QByteArray::fromBase64(q.value("user").toByteArray(), QByteArray::Base64UrlEncoding);
    purchasesNumber = q.value("purchasesNumber").toUInt();
    card = QByteArray::fromBase64(q.value("card").toByteArray(), QByteArray::Base64UrlEncoding);
    received = q.value("received").toUInt();
    _time = q.value("time").toInt();

    return true;
}

QString UsersCards::table() const {
    return "UsersData";
}

bool UsersCards::isValid() const {
    return user.size() && card.size();
}

QString UsersCards::condition() const {
    return QString("user='%0' AND card='%1'").arg(user.toBase64(QByteArray::Base64UrlEncoding),
                                                  card.toBase64(QByteArray::Base64UrlEncoding));
}

}
}
