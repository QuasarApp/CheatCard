//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "userscards.h"
#include "rci/rcutils.h"

namespace RC {
namespace API {
namespace V1 {


UsersCards::UsersCards(){
    _time = time(0);
}

UsersCards::UsersCards(unsigned int user, unsigned int card): V0::UsersCards(user, card) {}

UsersCards::UsersCards(const QSharedPointer<Interfaces::iUsersCards> &obj) {
    setUser(obj->getUser());
    setCard(obj->getCard());
    setPurchasesNumber(obj->getPurchasesNumber());
    setReceived(obj->getReceived());
    setCardVersion(obj->getCardVersion());
    setTime(obj->getRawTime());

    id = RC::RCUtils::makeUsersCardsId(user, card);
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

QSharedPointer<Interfaces::iUsersCards>
UsersCards::toObject(const QSharedPointer<Interfaces::iDB> &db) {
    if (!db)
        return nullptr;

    QSharedPointer<Interfaces::iUsersCards> result = db->makeEmptyUsersCard();

    result->setUser(user);
    result->setCard(card);
    result->setPurchasesNumber(purchasesNumber);
    result->setReceived(received);
    result->setCardVersion(cardVersion);
    result->setTime(_time);

    return result;
}

QDataStream &UsersCards::fromStream(QDataStream &stream) {


    QVariant plug;
    stream >> plug;
    stream >> user;
    stream >> card;
    stream >> id;
    stream >> purchasesNumber;
    stream >> received;
    stream >> cardVersion;

    return stream;
}

QDataStream &UsersCards::toStream(QDataStream &stream) const {
    QVariant plug;
    stream << plug;
    stream << user;
    stream << card;
    stream << id;
    stream << purchasesNumber;
    stream << received;
    stream << cardVersion;

    return stream;
}

}
}
}
