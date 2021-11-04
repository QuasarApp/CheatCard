
//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "dataconvertor.h"
#include "user.h"
#include "userheader.h"

namespace RC {
DataConvertor::DataConvertor()
{

}

QSharedPointer<User> DataConvertor::toUser(const QSharedPointer<UserHeader> &userHeader) {
    auto result = QSharedPointer<User>::create();

    result->setKey(userHeader->token());
    result->setId(userHeader->getUserId());
    result->setName(userHeader->userName());

    return  result;
}

QSharedPointer<UserHeader> DataConvertor::toUserHeader(const QSharedPointer<User> &user) {
    auto result = QSharedPointer<UserHeader>::create();

    result->setToken(user->getKey());
    result->setUserId(user->userId());
    result->setUserName(user->name());

    return  result;
}
}

