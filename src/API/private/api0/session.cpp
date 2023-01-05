//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "session.h"
namespace RC {
namespace API {
namespace V0 {

Session::Session(): QH::PKG::AbstractData() {
}

Session::Session(const QSharedPointer<Interfaces::iSession> & source) {
    sessionId = source->getSessionId();
    usercardId = source->getUsercardId();
}

long long Session::getSessionId() const {
    return sessionId;
}

void Session::setSessionId(long long newSessionId) {
    sessionId = newSessionId;
}

QDataStream &Session::fromStream(QDataStream &stream) {
//    DBObject::fromStream(stream);

    QVariant plug;
    stream >> plug;

    stream >> sessionId;
    stream >> usercardId;

    return stream;
}

QDataStream &Session::toStream(QDataStream &stream) const {
//    DBObject::toStream(stream);
    QVariant plug;
    stream << plug;


    stream << sessionId;
    stream << usercardId;

    return stream;
}

long long Session::getUsercardId() const {
    return usercardId;
}

void Session::setUsercardId(long long newUsercardId) {
    usercardId = newUsercardId;
}

QSharedPointer<Interfaces::iSession>
Session::toObject(const QSharedPointer<Interfaces::iDB> &db) {
    if (!db)
        return nullptr;

    QSharedPointer<Interfaces::iSession> result = db->makeEmptySession();

    result->setSessionId(sessionId);
    result->setUsercardId(usercardId);

    return result;
}

bool Session::isValid() const {
    return sessionId  && usercardId;
}

}
}
}
