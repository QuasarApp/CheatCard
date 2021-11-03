//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "userscards.h"

namespace RC {

UsersCards::UsersCards():QH::PKG::DBObject("UsersCards") {

}

UsersCards::UsersCards(unsigned int user, unsigned int card, bool owner): QH::PKG::DBObject("UsersCards") {
    this->user = user;
    this->card = card;
    this->owner = owner;
    id = genId(user, card);

}

QH::PKG::DBObject *UsersCards::createDBObject() const {
    return new UsersCards(0, 0);
}

QH::PKG::DBVariantMap UsersCards::variantMap() const {
    return {{"user",           {user,           QH::PKG::MemberType::Insert}},
            {"card",           {card,           QH::PKG::MemberType::Insert}},
            {"id",             {id,             QH::PKG::MemberType::PrimaryKey}},
            {"purchasesNumber",{purchasesNumber,QH::PKG::MemberType::InsertUpdate}},
            {"received",       {received,       QH::PKG::MemberType::InsertUpdate}},
            {"owner",          {owner,          QH::PKG::MemberType::InsertUpdate}},
            {"time",           {static_cast<int>(time(0)),      QH::PKG::MemberType::InsertUpdate}},
    };
}

QString UsersCards::primaryKey() const {
    return "id";
}

QDataStream &UsersCards::fromStream(QDataStream &stream) {

    DBObject::fromStream(stream);

    stream >> user;
    stream >> card;
    stream >> id;
    stream >> purchasesNumber;
    stream >> received;
    stream >> owner;
    stream >> cardVersion;

    return stream;
}

QDataStream &UsersCards::toStream(QDataStream &stream) const {
    DBObject::toStream(stream);

    stream << user;
    stream << card;
    stream << id;
    stream << purchasesNumber;
    stream << received;
    stream << owner;
    stream << cardVersion;

    return stream;
}

unsigned int UsersCards::getCardVersion() const {
    return cardVersion;
}

void UsersCards::setCardVersion(unsigned int newCardVersion) {
    cardVersion = newCardVersion;
}

long long UsersCards::getId() const {
    return id;
}

long long UsersCards::genId(unsigned int user, unsigned int card) {
    long long id = user;
    id = id << 32;
    id = id | card ;

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

bool UsersCards::getOwner() const {
    return owner;
}

void UsersCards::setOwner(bool newOwner) {
    owner = newOwner;
}

unsigned int UsersCards::getCard() const {
    return card;
}

void UsersCards::setCard(unsigned int newCard) {
    card = newCard;
    id = genId(user, card);
}

unsigned int UsersCards::getUser() const {
    return user;
}

void UsersCards::setUser(unsigned int newUser) {
    user = newUser;
    id = genId(user, card);
}

bool UsersCards::fromSqlRecord(const QSqlRecord &q) {

    id = q.value("id").toLongLong();
    user = q.value("user").toUInt();
    purchasesNumber = q.value("purchasesNumber").toUInt();
    card = q.value("card").toUInt();
    received = q.value("received").toUInt();
    owner = q.value("owner").toBool();

    return true;
}

bool UsersCards::isValid() const {
    return user && card && id;
}

}
