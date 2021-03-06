//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "session.h"
#include "time.h"
namespace RC {
namespace API {

Session::Session(): QH::PKG::DBObject("Sessions") {
}

QH::PKG::DBObject *Session::createDBObject() const {
    return new Session();
}

long long Session::getSessionId() const {
    return sessionId;
}

void Session::setSessionId(long long newSessionId) {
    sessionId = newSessionId;
}

QH::PKG::DBVariantMap Session::variantMap() const {
    return {
            {"id",             {sessionId,       QH::PKG::MemberType::Insert}},
            {"usersCardsID",   {usercardId,      QH::PKG::MemberType::Insert}},
            {"time",           {static_cast<int>(time(0)),      QH::PKG::MemberType::Insert}},

    };
}

QString Session::primaryKey() const {
    return "id";
}

QDataStream &Session::fromStream(QDataStream &stream) {
    DBObject::fromStream(stream);

    stream >> sessionId;
    stream >> usercardId;

    return stream;
}

QDataStream &Session::toStream(QDataStream &stream) const {
    DBObject::toStream(stream);

    stream << sessionId;
    stream << usercardId;

    return stream;
}

QString Session::condition() const {

    if (usercardId) {
        return QString("id = '%0' AND usersCardsID = '%1'").
                arg(sessionId).arg(usercardId);
    }

    return QString("id = '%0'").arg(sessionId);
}

long long Session::getUsercardId() const {
    return usercardId;
}

void Session::setUsercardId(long long newUsercardId) {
    usercardId = newUsercardId;
}

bool Session::fromSqlRecord(const QSqlRecord &q) {

    usercardId = q.value("usersCardsID").toLongLong();
    sessionId = q.value("id").toLongLong();

    return true;
}

bool Session::isValid() const {
    return sessionId  && usercardId;
}

}
}
