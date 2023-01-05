//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "userscards.h"
#include "rci/core/idb.h"
#include "rci/rcutils.h"


namespace RC {
namespace API {
namespace V0 {

#pragma pack(push, 1)
struct UserCardId {
    unsigned int cardId;
    unsigned int userId;
};
#pragma pack(pop)

UsersCards::UsersCards():QH::PKG::AbstractData() {

}

UsersCards::UsersCards(const QSharedPointer<Interfaces::iUsersCards> &obj) {
    setUser(obj->getUser());
    setCard(obj->getCard());
    setPurchasesNumber(obj->getPurchasesNumber());
    setReceived(obj->getReceived());
    setCardVersion(obj->getCardVersion());
    id = RC::RCUtils::makeUsersCardsId(user, card);

}

UsersCards::UsersCards(unsigned int user, unsigned int card) {
    this->user = user;
    this->card = card;
    id = RC::RCUtils::makeUsersCardsId(user, card);

}

QDataStream &UsersCards::fromStream(QDataStream &stream) {

    //    DBObject::fromStream(stream);

    QVariant plug;
    stream >> plug;

    stream >> user;
    stream >> card;
    stream >> id;
    stream >> purchasesNumber;
    stream >> received;

    bool owner;
    stream >> owner;
    stream >> cardVersion;

    return stream;
}

QDataStream &UsersCards::toStream(QDataStream &stream) const {
    //    DBObject::toStream(stream);
    QVariant plug;
    stream << plug;

    stream << user;
    stream << card;
    stream << id;
    stream << purchasesNumber;
    stream << received;

    bool owner(false);
    stream << owner;
    stream << cardVersion;

    return stream;
}

void UsersCards::setTime(unsigned int newTime) {
    _time = newTime;
}

QSharedPointer<RC::Interfaces::iUsersCards>
UsersCards::toObject(const QSharedPointer<Interfaces::iDB> &db) {

    if (!db)
        return nullptr;

    QSharedPointer<Interfaces::iUsersCards> result = db->makeEmptyUsersCard();

    result->setUser(user);
    result->setCard(card);
    result->setPurchasesNumber(purchasesNumber);
    result->setReceived(received);
    result->setCardVersion(cardVersion);

    return result;
}

QDateTime UsersCards::getTime() const {
    return QDateTime::fromSecsSinceEpoch(_time);
}

bool UsersCards::isActive() const {
    return QDateTime::currentDateTime() <
           getTime().addSecs(Interfaces::ACTIVE_USER_TIME_LIMIT);
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
    id = RCUtils::makeUsersCardsId(user, card);
}

unsigned int UsersCards::getUser() const {
    return user;
}

void UsersCards::setUser(unsigned int newUser) {
    user = newUser;
    id = RCUtils::makeUsersCardsId(user, card);
}

bool UsersCards::isValid() const {
    return user && card && id;
}

}
}
}
