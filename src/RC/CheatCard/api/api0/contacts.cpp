
//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "contacts.h"

namespace RC {
namespace API {

Contacts::Contacts(): QH::PKG::DBObject("Contacts") {}

QH::PKG::DBObject *Contacts::createDBObject() const {
    return new Contacts();
}

QH::PKG::DBVariantMap Contacts::variantMap() const {
    return {{"user",               {user,         QH::PKG::MemberType::Insert}},
            {"childuserId",        {childUserId,  QH::PKG::MemberType::Insert}},
            {"genesisKey",         {genesisKey,   QH::PKG::MemberType::Insert}},
            {"info",               {info,         QH::PKG::MemberType::InsertUpdate}},

    };
}

bool Contacts::isValid() const {
    return user && childUserId && genesisKey;
}

QString Contacts::primaryKey() const {
    return "";
}

QDataStream &Contacts::fromStream(QDataStream &stream) {

    stream >> user;
    stream >> childUserId;
    stream >> genesisKey;
    stream >> info;

    return stream;
}

QDataStream &Contacts::toStream(QDataStream &stream) const {

    stream << user;
    stream << childUserId;
    stream << genesisKey;
    stream << info;

    return stream;
}

QString Contacts::condition() const {
    return QString("user='%0' AND genesisKey='%1'").arg(user).arg(genesisKey);
}

int Contacts::getGenesisKey() const {
    return genesisKey;
}

void Contacts::setGenesisKey(int newGenesisKey) {
    genesisKey = newGenesisKey;
}

unsigned int Contacts::getChildUserId() const {
    return childUserId;
}

void Contacts::setChildUserId(unsigned int  newPermisionKey) {
    childUserId = newPermisionKey;
}

unsigned int Contacts::getUser() const {
    return user;
}

void Contacts::setUser(unsigned int newUser) {
    user = newUser;
}

const QString &Contacts::getInfo() const {
    return info;
}

void Contacts::setInfo(const QString &newInfo) {
    info = newInfo;
}

bool Contacts::fromSqlRecord(const QSqlRecord &q) {

    childUserId = q.value("childuserId").toUInt();
    genesisKey = q.value("genesisKey").toInt();

    user = q.value("user").toUInt();
    info = q.value("info").toUInt();

    return true;
}

}
}
