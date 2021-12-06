//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SERVERSSL_H
#define SERVERSSL_H

#include "abstractnode.h"
#include "server.h"
#include "CheatCard/clearolddata.h"

#include <isqldbcache.h>

namespace RC {

class CHEATCARD_CORE_EXPORT ServerSSL: public Server
{
    Q_OBJECT
public:
    ServerSSL(QH::ISqlDBCache *db);

};
}
#endif // SERVERSSL_H
