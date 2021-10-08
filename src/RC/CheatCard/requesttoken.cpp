//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "requesttoken.h"

namespace RC {

RequestToken::RequestToken()
{

}

unsigned long long RequestToken::requestToken() const {
    return _requestToken;
}

void RequestToken::setRequestToken(unsigned long long newRequestToken) {
    _requestToken = newRequestToken;
}
}
