//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "userscards.h"

namespace RC {
namespace API {
namespace V3 {

UsersCards::UsersCards() {}
UsersCards::UsersCards(const QSharedPointer<Interfaces::iUsersCards>& obj) {
    UsersCards::fromObject(obj);
};

UsersCards::UsersCards(const QByteArray &user, const QByteArray &card) {
    UsersCards::setUser(user);
    UsersCards::setCard(card);
}

QDataStream &UsersCards::fromStream(QDataStream &stream) {

    stream >> user;
    stream >> card;
    stream >> purchasesNumber;
    stream >> received;
    stream >> cardVersion;
    stream >> _time;

    return stream;
}

QDataStream &UsersCards::toStream(QDataStream &stream) const {

    stream << user;
    stream << card;
    stream << purchasesNumber;
    stream << received;
    stream << cardVersion;
    stream << _time;

    return stream;
}

QByteArray UsersCards::getUser() const {
    return user;
}

void UsersCards::setUser(const QByteArray &newUser) {
    user = newUser;
}

QByteArray UsersCards::getCard() const {
    return card;
}

void UsersCards::setCard(const QByteArray &newCard) {
    card = newCard;
}

unsigned int UsersCards::getCardVersion() const {
    return cardVersion;
}

void UsersCards::setCardVersion(unsigned int newCardVersion) {
    cardVersion = newCardVersion;
}

unsigned int UsersCards::getPurchasesNumber() const {
    return purchasesNumber;
}

void UsersCards::setPurchasesNumber(unsigned int newPurchasesNumber) {
    purchasesNumber = newPurchasesNumber;
}

unsigned int UsersCards::getReceived() const {
    return received;
}

void UsersCards::setReceived(unsigned int newReceived) {
    received = newReceived;
}

unsigned int UsersCards::time() const {
    return _time;
}

void UsersCards::setTime(unsigned int newTime) {
    _time = newTime;
}

bool RC::API::V3::UsersCards::UsersCards::isValid() const {
    return user.size() && card.size();
}

QSharedPointer<Interfaces::iUsersCards>
UsersCards::toObject(const QSharedPointer<Interfaces::iDB> &db) const {
    if (!db)
        return nullptr;


    QSharedPointer<Interfaces::iUsersCards> result = db->makeEmptyUsersCard();
    result->setCard(getCard());
    result->setCardVersion(getCardVersion());
    result->setPurchasesNumber(getPurchasesNumber());
    result->setReceived(getReceived());
    result->setTime(time());
    result->setUser(getUser());

    return result;
}

void UsersCards::fromObject(const QSharedPointer<Interfaces::iUsersCards> &obj) {

    if (obj) {

        UsersCards::setUser(obj->getUser());
        UsersCards::setCard(obj->getCard());
        UsersCards::setPurchasesNumber(obj->getPurchasesNumber());
        UsersCards::setReceived(obj->getReceived());
        UsersCards::setCardVersion(obj->getCardVersion());
        UsersCards::setTime(obj->getRawTime());
    }
}

}
}
}
