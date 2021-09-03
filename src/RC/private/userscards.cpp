//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "userscards.h"

namespace RC {

UsersCards::UsersCards(int user, int card, bool owner): QH::PKG::DBObject("UsersCards") {
    this->user = user;
    this->card = card;
    this->owner = owner;
}

QH::PKG::DBObject *UsersCards::createDBObject() const {
    return new UsersCards(0, 0);
}

QH::PKG::DBVariantMap UsersCards::variantMap() const {
    return {{"user",      {user,        QH::PKG::MemberType::Insert}},
            {"card",      {card,        QH::PKG::MemberType::Insert}},
            {"owner",     {owner,       QH::PKG::MemberType::Insert}},
    };
}

QString UsersCards::primaryKey() const {
    return "";
}

bool UsersCards::fromSqlRecord(const QSqlRecord &q) {

    user = q.value("user").toInt();
    card = q.value("card").toInt();
    owner = q.value("owner").toBool();

    return true;
}

bool UsersCards::isValid() const {
    return user >= 0 && card >= 0;
}

}
