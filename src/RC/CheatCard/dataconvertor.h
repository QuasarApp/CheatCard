//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef DATACONVERTOR_H
#define DATACONVERTOR_H

#include <QSharedPointer>
#include "core_global.h"

namespace RC {

namespace API {
class UserHeader;
class User;
}

class CHEATCARD_CORE_EXPORT DataConvertor
{
public:
    DataConvertor();

    static QSharedPointer<API::User> toUser(const QSharedPointer<RC::API::UserHeader>& userHeader);
    static QSharedPointer<API::User> toUser(const RC::API::UserHeader& userHeader);

    static QSharedPointer<API::UserHeader> toUserHeader(const QSharedPointer<RC::API::User> &user);

};
}


#endif // DATACONVERTOR_H
