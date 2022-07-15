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
    _time = time(0);
}

UsersCards::UsersCards(unsigned int user, unsigned int card): APIv1::UsersCards() {
    this->user = user;
    this->card = card;
    id = genId(user, card);

}

QH::PKG::DBObject *UsersCards::createDBObject() const {
    return new UsersCards(0, 0);
}

QH::PKG::DBVariantMap UsersCards::variantMap() const {
    return {{"user",           {user,            QH::PKG::MemberType::Insert}},
            {"card",           {card,            QH::PKG::MemberType::Insert}},
            {"id",             {id,              QH::PKG::MemberType::PrimaryKey}},
            {"purchasesNumber",{purchasesNumber, QH::PKG::MemberType::InsertUpdate}},
            {"received",       {received,        QH::PKG::MemberType::InsertUpdate}},
            {"time",           {_time,           QH::PKG::MemberType::InsertUpdate}},
    };
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
