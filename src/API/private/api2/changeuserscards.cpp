//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "changeuserscards.h"
#include "rci/rcutils.h"


namespace RC {
namespace API {
namespace V2 {

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

unsigned int ChangeUsersCards::getUser() const {
    return RC::RCUtils::getUserIdFromUsrsCards(getUsercardId());
}

unsigned int ChangeUsersCards::getCard() const {
    return RC::RCUtils::getCardIdFromUsrsCards(getUsercardId());
}

bool ChangeUsersCards::isValid() const {
    return V0::Session::isValid() && _secret.size();
}

QDataStream &ChangeUsersCards::fromStream(QDataStream &stream) {
    V0::Session::fromStream(stream);

    stream >> _receive;
    stream >> _purchase;
    stream >> _secret;

    return stream;
}

QDataStream &ChangeUsersCards::toStream(QDataStream &stream) const {
    V0::Session::toStream(stream);

    stream << _receive;
    stream << _purchase;
    stream << _secret;

    return stream;
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
