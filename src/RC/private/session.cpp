//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "session.h"

namespace RC {

Session::Session(): QH::PKG::DBObject("Sessions") {

}

QH::PKG::DBObject *Session::createDBObject() const {
    return new Session();
}

unsigned long long Session::getSessionId() const {
    return sessionId;
}

void Session::setSessionId(unsigned long long newSessionId) {
    sessionId = newSessionId;
}

QH::PKG::DBVariantMap Session::variantMap() const {
    return {
            {"id",             {usercardId,     QH::PKG::MemberType::Insert}},
            {"sessionID",      {sessionId,      QH::PKG::MemberType::Insert}},
    };
}

QString Session::primaryKey() const {
    return "id";
}

unsigned long long Session::getUsercardId() const {
    return usercardId;
}

void Session::setUsercardId(unsigned long long newUsercardId) {
    usercardId = newUsercardId;
}

bool Session::fromSqlRecord(const QSqlRecord &q) {

    usercardId = q.value("usersCardsID").toULongLong();
    sessionId = q.value("id").toULongLong();

    return true;
}

bool Session::isValid() const {
    return sessionId > 0 && usercardId > 0xFFFFFFFF;
}

}

