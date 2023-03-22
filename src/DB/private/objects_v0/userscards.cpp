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
namespace DBv0 {


UsersCards::UsersCards() {
}

QH::PKG::DBObject *UsersCards::createDBObject() const {
    return new UsersCards();
}

QH::PKG::DBVariantMap UsersCards::variantMap() const {
    return {{"user",           {user,            QH::PKG::MemberType::Insert}},
            {"card",           {card,            QH::PKG::MemberType::Insert}},
            {"id",             {id,              QH::PKG::MemberType::PrimaryKey}},
            {"purchasesNumber",{purchasesNumber, QH::PKG::MemberType::InsertUpdate}},
            {"received",       {received,        QH::PKG::MemberType::InsertUpdate}},
            {"time",           {_time,           QH::PKG::MemberType::InsertUpdate}},
            };
}

QString UsersCards::primaryKey() const {
    return "id";
}

QVariant UsersCards::primaryValue() const {
    return id;
}

QString UsersCards::toString() const {
    QString result("id: %0 \n"
                   "user: %1 \n"
                   "card: %2 \n"
                   "purchasesNumber: %3 \n"
                   "received: %4 \n"
                   "time: %5 \n ");

    result = result.arg(id).
             arg(user).
             arg(card).
             arg(purchasesNumber).
             arg(received).
             arg(_time);

    return result;
}

QDataStream &UsersCards::fromStream(QDataStream &stream) {

    DBObject::fromStream(stream); // clazy:exclude=skipped-base-method

    stream >> user;
    stream >> card;
    stream >> id;
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
    stream << id;
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

long long UsersCards::getId() const {
    return id;
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

unsigned int UsersCards::getCard() const {
    return card;
}

void UsersCards::setCard(unsigned int newCard) {
    card = newCard;
}

unsigned int UsersCards::getUser() const {
    return user;
}

void UsersCards::setUser(unsigned int newUser) {
    user = newUser;
}

bool UsersCards::fromSqlRecord(const QSqlRecord &q) {

    id = q.value("id").toLongLong();
    user = q.value("user").toUInt();
    purchasesNumber = q.value("purchasesNumber").toUInt();
    card = q.value("card").toUInt();
    received = q.value("received").toUInt();
    _time = q.value("time").toInt();

    return true;
}

QString UsersCards::table() const {
    return "UsersCards";
}

bool UsersCards::isValid() const {
    return user && card && id;
}

}
}
