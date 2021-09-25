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
#include <userscards.h>
#include <card.h>
#include "CheatCard_global.h"

namespace RC {


Visitor::Visitor(QH::ISqlDBCache *db): BaseNode(db) {
    registerPackageType<UsersCards>();
    registerPackageType<Card>();

}

bool Visitor::checkCardData(unsigned long long session,
                            const QString &domain, int port) {
    if (session <= 0)
        return false;

    _lastRequested = session;

    return addNode(domain, port);
}

void Visitor::nodeConfirmend(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConfirmend(node);

    CardStatusRequest request;

    request.setSessionId(_lastRequested);

    sendData(&request, node->networkAddress());
}

}
