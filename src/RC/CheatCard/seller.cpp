//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "seller.h"

#include <session.h>
namespace RC {

Seller::Seller(QH::ISqlDBCache *db): BaseNode(db) {
}

bool Seller::sendLastSession(unsigned long long session, const QString &domain, int port) {
    if (session <= 0)
        return false;

    _lastRequested = session;
    return addNode(domain, port);
}

void Seller::nodeConfirmend(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConfirmend(node);

    Session session;

    session.setSessionId(_lastRequested);

    sendData(&session, node->networkAddress());
}
}
