//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "datastructures.h"

namespace RC {

UserHeader::UserHeader() {

}

bool UserHeader::isValid() const {
    return _token.size() == 32 && userId > 0 && sessionId > 0 && AbstractData::isValid();;
}

QDataStream &UserHeader::fromStream(QDataStream &stream) {
    AbstractData::fromStream(stream);

    stream >> _token;
    stream >> sessionId;
    stream >> userId;

    return stream;
}

QDataStream &UserHeader::toStream(QDataStream &stream) const {
    AbstractData::toStream(stream);

    stream << _token;
    stream << sessionId;
    stream << userId;

    return stream;
}

const QByteArray &UserHeader::token() const {
    return _token;
}

void UserHeader::setToken(const QByteArray &newToken) {
    _token = newToken;
}

unsigned long long UserHeader::getSessionId() const {
    return sessionId;
}

void UserHeader::setSessionId(unsigned long long newSessionId) {
    sessionId = newSessionId;
}

unsigned int UserHeader::getUserId() const {
    return userId;
}

void UserHeader::setUserId(unsigned int newUserId) {
    userId = newUserId;
}

CardStatusRequest::CardStatusRequest() {

}

bool CardStatusRequest::isValid() const {
    return sessionId > 0 && AbstractData::isValid();
}

QDataStream &CardStatusRequest::fromStream(QDataStream &stream) {

    AbstractData::fromStream(stream);

    stream >> sessionId;

    return stream;
}

QDataStream &CardStatusRequest::toStream(QDataStream &stream) const {
    AbstractData::toStream(stream);

    stream << sessionId;

    return stream;
}

unsigned long long CardStatusRequest::getSessionId() const {
    return sessionId;
}

void CardStatusRequest::setSessionId(unsigned long long newSessionId) {
    sessionId = newSessionId;
}

bool CardDataRequest::isValid() const {
    return AbstractData::isValid() && cardId;
}

QDataStream &CardDataRequest::fromStream(QDataStream &stream) {
    AbstractData::fromStream(stream);

    stream >> cardId;

    return stream;
}

QDataStream &CardDataRequest::toStream(QDataStream &stream) const {
    AbstractData::toStream(stream);

    stream << cardId;

    return stream;
}

unsigned int CardDataRequest::getCardId() const {
    return cardId;
}

void CardDataRequest::setCardId(unsigned int newCardId) {
    cardId = newCardId;
}

RC::CardDataRequest::CardDataRequest() {}

}
