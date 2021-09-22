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

unsigned int Session::getUser() const {
    return user;
}

void Session::setUser(unsigned int newUser) {
    user = newUser;
}

unsigned int Session::getCard() const {
    return card;
}

void Session::setCard(unsigned int newCard) {
    card = newCard;
}

const QByteArray &Session::getSessionId() const {
    return sessionId;
}

void Session::setSessionId(const QByteArray &newSessionId) {
    sessionId = newSessionId;
}

QH::PKG::DBVariantMap Session::variantMap() const {
    return {{"user",           {user,           QH::PKG::MemberType::Insert}},
            {"card",           {card,           QH::PKG::MemberType::Insert}},
            {"sessionID",      {sessionId,      QH::PKG::MemberType::Insert}},
    };
}

QString Session::primaryKey() const {
    return "";
}

bool Session::fromSqlRecord(const QSqlRecord &q) {

    user = q.value("user").toUInt();
    card = q.value("card").toUInt();
    sessionId = q.value("sessionID").toByteArray();

    return true;
}

bool Session::isValid() const {
    return sessionId.size() == 32 && user != 0 && card != 0;
}

}

