//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "userscards.h"

namespace RC {
namespace APIv1_5 {

UsersCards::UsersCards(): APIv1::UsersCards() {
}

UsersCards::UsersCards(unsigned int user, unsigned int card): APIv1::UsersCards(user, card) {

}

QH::PKG::DBObject *UsersCards::createDBObject() const {
    return new UsersCards(0, 0);
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

}
}
