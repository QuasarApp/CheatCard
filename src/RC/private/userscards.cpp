//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
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
    };
}

QString UsersCards::primaryKey() const {
    return "id";
}

unsigned long long UsersCards::getId() const {
    return id;
}

unsigned long long UsersCards::genId(unsigned int user, unsigned int card) {
    unsigned long long id = user;
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

int UsersCards::getCard() const {
    return card;
}

void UsersCards::setCard(int newCard) {
    card = newCard;
    id = genId(user, card);
}

int UsersCards::getUser() const {
    return user;
}

void UsersCards::setUser(int newUser) {
    user = newUser;
    id = genId(user, card);
}

bool UsersCards::fromSqlRecord(const QSqlRecord &q) {

    id = q.value("id").toULongLong();
    user = q.value("user").toUInt();
    purchasesNumber = q.value("purchasesNumber").toUInt();
    card = q.value("card").toUInt();
    received = q.value("received").toUInt();
    owner = q.value("owner").toBool();

    return true;
}

bool UsersCards::isValid() const {
    return user != 0 && card != 0 && id > 0xFFFFFFFF;
}

}
