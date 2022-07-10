
//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "dataconvertor.h"
#include "usersnames.h"
#include "CheatCard/api/api0/user.h"
#include "CheatCard/api/api0/userheader.h"

namespace RC {
DataConvertor::DataConvertor()
{

}

QSharedPointer<API::User> DataConvertor::toUser(const QSharedPointer<API::UserHeader> &userHeader) {
    auto result = QSharedPointer<API::User>::create();

    result->setKey(userHeader->token());
    result->setId(userHeader->getUserId());
    result->setName(userHeader->userName());

    return  result;
}

QSharedPointer<API::UserHeader> DataConvertor::toUserHeader(const QSharedPointer<API::User> &user) {
    auto result = QSharedPointer<API::UserHeader>::create();

    result->setToken(user->getKey());
    result->setUserId(user->userId());
    result->setUserName(user->name());

    return  result;
}
}

