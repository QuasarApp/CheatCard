//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "sallerscards.h"

namespace RC {

SallersCards::SallersCards(int saller, int card): QH::PKG::DBObject("SallersCards") {
    this->saller = saller;
    this->card = card;
}

QH::PKG::DBObject *SallersCards::createDBObject() const {
    return new SallersCards(0, 0);
}

QH::PKG::DBVariantMap SallersCards::variantMap() const {
    return {{"saller",      {saller,        QH::PKG::MemberType::Insert}},
            {"card",        {card,          QH::PKG::MemberType::Insert}},
    };
}

bool SallersCards::fromSqlRecord(const QSqlRecord &q) {

    saller = q.value("saller").toInt();
    card = q.value("card").toInt();

    return true;
}

QString SallersCards::primaryKey() const {
    return "";
}

}
