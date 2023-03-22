//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "changeuserscards.h"

namespace RC {
namespace API {
namespace V3 {

ChangeUsersCards::ChangeUsersCards() {

}

ChangeUsersCards::~ChangeUsersCards() {

}

qint8 ChangeUsersCards::receive() const {
    return _receive;
}

void ChangeUsersCards::setReceive(qint8 newReceive) {
    _receive = newReceive;
}

qint8 ChangeUsersCards::purchase() const {
    return _purchase;
}

void ChangeUsersCards::setPurchase(qint8 newPurchase) {
    _purchase = newPurchase;
}

const QByteArray& ChangeUsersCards::getUser() const {
    return _user;
}

const QByteArray& ChangeUsersCards::getCard() const {
    return _card;
}

bool ChangeUsersCards::isValid() const {
    return _secret.size() && _user.size() && _card.size();
}

QDataStream &ChangeUsersCards::fromStream(QDataStream &stream) {

    stream >> _receive;
    stream >> _purchase;
    stream >> _secret;
    stream >> _user;
    stream >> _card;

    return stream;
}

QDataStream &ChangeUsersCards::toStream(QDataStream &stream) const {

    stream << _receive;
    stream << _purchase;
    stream << _secret;
    stream << _user;
    stream << _card;

    return stream;
}

void ChangeUsersCards::setUser(const QByteArray &newUser) {
    _user = newUser;
}

void ChangeUsersCards::setCard(const QByteArray &newCard) {
    _card = newCard;
}

const QByteArray &ChangeUsersCards::secret() const {
    return _secret;
}

void ChangeUsersCards::setSecret(const QByteArray &newSecret) {
    _secret = newSecret;
}


}
}
}
