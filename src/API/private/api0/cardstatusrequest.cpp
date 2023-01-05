//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "cardstatusrequest.h"

namespace RC {
namespace API {
namespace V0 {

CardStatusRequest::CardStatusRequest() {

}

bool CardStatusRequest::isValid() const {
    return sessionId && AbstractData::isValid();
}

QDataStream &CardStatusRequest::fromStream(QDataStream &stream) {

    RequestToken::fromStream(stream);

    stream >> sessionId;

    return stream;
}

QDataStream &CardStatusRequest::toStream(QDataStream &stream) const {
    RequestToken::toStream(stream);

    stream << sessionId;

    return stream;
}

long long CardStatusRequest::getSessionId() const {
    return sessionId;
}

void CardStatusRequest::setSessionId(long long newSessionId) {
    sessionId = newSessionId;
}
}
}
}
