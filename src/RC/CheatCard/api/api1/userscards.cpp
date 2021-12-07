//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "userscards.h"

namespace RC {
namespace APIv1 {

UsersCards::UsersCards(): API::UsersCards() {

}

UsersCards::UsersCards(unsigned int user, unsigned int card): API::UsersCards(user, card) {
    this->user = user;
    this->card = card;
    id = genId(user, card);

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

    return stream;
}

}
}
