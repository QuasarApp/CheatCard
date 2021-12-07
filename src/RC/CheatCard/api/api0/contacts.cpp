
//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "contacts.h"

namespace RC {
namespace API {

Contacts::Contacts(unsigned int user, unsigned int contactUser ): QH::PKG::DBObject("Contacts") {
    this->contactUser = contactUser;
    this->user = user;
}

QH::PKG::DBObject *Contacts::createDBObject() const {
    return new Contacts(0, 0);
}

QH::PKG::DBVariantMap Contacts::variantMap() const {
    return {{"user",               {user,        QH::PKG::MemberType::Insert}},
            {"contactUser",        {contactUser, QH::PKG::MemberType::Insert}},
    };
}

QString Contacts::primaryKey() const {
    return "";
}

bool Contacts::fromSqlRecord(const QSqlRecord &q) {

    contactUser = q.value("contactUser").toUInt();
    user = q.value("user").toUInt();

    return true;
}

}
}