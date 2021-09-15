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
}

QH::PKG::DBObject *UsersCards::createDBObject() const {
    return new UsersCards(0, 0);
}

QH::PKG::DBVariantMap UsersCards::variantMap() const {
    return {{"user",           {user,           QH::PKG::MemberType::Insert}},
            {"card",           {card,           QH::PKG::MemberType::Insert}},
            {"purchasesNumber",{purchasesNumber,QH::PKG::MemberType::InsertUpdate}},
            {"owner",          {owner,          QH::PKG::MemberType::InsertUpdate}},
    };
}

QString UsersCards::primaryKey() const {
    return "";
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
}

int UsersCards::getUser() const {
    return user;
}

void UsersCards::setUser(int newUser) {
    user = newUser;
}

bool UsersCards::fromSqlRecord(const QSqlRecord &q) {

    user = q.value("user").toUInt();
    purchasesNumber = q.value("purchasesNumber").toUInt();
    card = q.value("card").toUInt();

    owner = q.value("owner").toBool();

    return true;
}

bool UsersCards::isValid() const {
    return user != 0 && card != 0;
}

QString UsersCards::condition() const {
    return QString("user=%0 AND card=%1").arg(user).arg(card);
}

}
