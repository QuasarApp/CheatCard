//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "userscards.h"

namespace RC {
namespace API {
namespace V2 {

UsersCards::UsersCards() {}
UsersCards::UsersCards(const QSharedPointer<Interfaces::iUsersCards>& obj): V1::UsersCards(obj) {};

UsersCards::UsersCards(unsigned int user, unsigned int card): API::V1::UsersCards(user, card) {}
QDataStream &UsersCards::fromStream(QDataStream &stream) {

    QVariant plug;
    stream >> plug;
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
    QVariant plug;
    stream << plug;
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
}
