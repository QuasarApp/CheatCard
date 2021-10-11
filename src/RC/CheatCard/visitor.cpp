//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "cardstatusrequest.h"
#include "visitor.h"

#include <CheatCard/user.h>
#include <cstring>
#include <CheatCard/session.h>
#include <CheatCard/userscards.h>
#include <CheatCard/card.h>

namespace RC {


Visitor::Visitor(QH::ISqlDBCache *db): BaseNode(db) {
    registerPackageType<UsersCards>();
    registerPackageType<Card>();

}

bool Visitor::checkCardData(long long session,
                            const QString &domain, int port) {
    if (!session)
        return false;

    _lastRequested = session;

    return addNode(domain, port);
}


void Visitor::nodeConnected(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConnected(node);

    CardStatusRequest request;

    request.setSessionId(_lastRequested);

    sendData(&request, node->networkAddress());
}

}
