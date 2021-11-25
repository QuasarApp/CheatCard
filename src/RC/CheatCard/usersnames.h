//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef USERSNAMES_H
#define USERSNAMES_H

#include "core_global.h"
#include <QObject>


class CHEATCARD_CORE_EXPORT UsersNames {

public:
    UsersNames();

    static QString randomUserName();
};

#endif // USERSNAMES_H
