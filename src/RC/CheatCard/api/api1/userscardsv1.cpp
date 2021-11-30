//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "userscardsv1.h"

namespace RC {

UsersCardsV1::UsersCardsV1():UsersCards() {}

UsersCardsV1::UsersCardsV1(unsigned int user, unsigned int card):
    UsersCards(user, card) {}

QH::PKG::DBObject *UsersCardsV1::createDBObject() const {
    return new UsersCardsV1(0, 0);
}

QDataStream &UsersCardsV1::fromStream(QDataStream &stream) {


    DBObject::fromStream(stream); // clazy:exclude=skipped-base-method

    stream >> user;
    stream >> card;
    stream >> id;
    stream >> purchasesNumber;
    stream >> received;
    stream >> cardVersion;

    return stream;
}

QDataStream &UsersCardsV1::toStream(QDataStream &stream) const {
    DBObject::toStream(stream); // clazy:exclude=skipped-base-method

    stream << user;
    stream << card;
    stream << id;
    stream << purchasesNumber;
    stream << received;
    stream << cardVersion;

    return stream;
}



}
