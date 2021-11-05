//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef DATACONVERTOR_H
#define DATACONVERTOR_H

#include <QSharedPointer>

namespace RC {

class UserHeader;
class User;

class DataConvertor
{
public:
    DataConvertor();

    static QSharedPointer<User> toUser(const QSharedPointer<UserHeader>& userHeader);
    static QSharedPointer<UserHeader> toUserHeader(const QSharedPointer<User>& user);

};
}


#endif // DATACONVERTOR_H
