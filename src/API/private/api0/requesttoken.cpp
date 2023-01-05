//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "requesttoken.h"

namespace RC {
namespace API {
namespace V0 {

RequestToken::RequestToken()
{

}

unsigned long long RequestToken::requestToken() const {
    return _requestToken;
}

void RequestToken::setRequestToken(unsigned long long newRequestToken) {
    _requestToken = newRequestToken;
}

unsigned long long RequestToken::responceToken() const {
    return _responceToken;
}

void RequestToken::setResponceToken(unsigned long long newResponceToken) {
    _responceToken = newResponceToken;
}

QDataStream &RequestToken::fromStream(QDataStream &stream) {

    stream >> _responceToken;
    stream >> _requestToken;

    return stream;
}

QDataStream &RequestToken::toStream(QDataStream &stream) const {
    stream << _responceToken;
    stream << _requestToken;

    return stream;
}
}
}
}
