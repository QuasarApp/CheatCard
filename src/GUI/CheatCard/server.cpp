//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "server.h"

#include <card.h>
#include <datastructures.h>
#include <session.h>
#include <userscards.h>

namespace RC {

Server::Server(QH::ISqlDBCache *db): BaseNode(db) {
    registerPackageType<Session>();
    registerPackageType<CardStatusRequest>();
    registerPackageType<UsersCards>();
    registerPackageType<CardDataRequest>();
    registerPackageType<Card>();

}

}
