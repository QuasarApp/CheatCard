
//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "contacts.h"

namespace RC {

Contacts::Contacts(int saller, int user ): QH::PKG::DBObject("Contacts") {
    this->saller = saller;
    this->user = user;
}

QH::PKG::DBObject *Contacts::createDBObject() const {
    return new Contacts(0, 0);
}

QH::PKG::DBVariantMap Contacts::variantMap() const {
    return {{"saller",      {saller,        QH::PKG::MemberType::Insert}},
            {"user",        {user,          QH::PKG::MemberType::Insert}},
    };
}

QString Contacts::primaryKey() const {
    return "";
}

bool Contacts::fromSqlRecord(const QSqlRecord &q) {

    saller = q.value("saller").toInt();
    user = q.value("user").toInt();

    return true;
}

}
