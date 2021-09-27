//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "server.h"

#include <CheatCard/card.h>
#include <CheatCard/datastructures.h>
#include <CheatCard/session.h>
#include <CheatCard/userscards.h>

namespace RC {

Server::Server(QH::ISqlDBCache *db): BaseNode(db) {
    registerPackageType<Session>();
    registerPackageType<CardStatusRequest>();
    registerPackageType<UsersCards>();
    registerPackageType<CardDataRequest>();
    registerPackageType<Card>();

}

}
