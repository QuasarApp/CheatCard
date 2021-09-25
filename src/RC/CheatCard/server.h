//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SERVER_H
#define SERVER_H

#include "abstractnode.h"
#include "basenode.h"

#include <isqldbcache.h>

namespace RC {

class Server: public BaseNode
{
    Q_OBJECT
public:
    Server(QH::ISqlDBCache *db);

};
}
#endif // SERVER_H
