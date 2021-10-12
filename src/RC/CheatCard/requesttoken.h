//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef REQUESTTOKEN_H
#define REQUESTTOKEN_H

#include "core_global.h"

namespace RC {

class CHEATCARD_CORE_EXPORT RequestToken {
public:
    RequestToken();
    unsigned long long requestToken() const;
    void setRequestToken(unsigned long long newRequestToken);
protected:
    unsigned long long _requestToken = 0;

};
}
#endif // REQUESTTOKEN_H
