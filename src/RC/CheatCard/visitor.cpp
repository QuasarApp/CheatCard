//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "visitor.h"

#include <datastructures.h>
#include <user.h>
#include <cstring>
#include <session.h>
#include "itargetnode.h"

namespace RC {

Visitor::Visitor(QH::ISqlDBCache *db, QSharedPointer<User> newActiveUser):
    IConnectorBackEnd(db) {

    setMode(Client);
    setActiveUser(newActiveUser);
}

bool Visitor::hello() {

    return sendStatusRequest(lastSessionId());
}
}
