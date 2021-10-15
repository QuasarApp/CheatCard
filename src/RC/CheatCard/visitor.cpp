//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "cardstatusrequest.h"
#include "nodeinfo.h"
#include "visitor.h"

#include <CheatCard/user.h>
#include <cstring>
#include <CheatCard/session.h>
#include <CheatCard/userscards.h>
#include <CheatCard/card.h>

namespace RC {


Visitor::Visitor(QH::ISqlDBCache *db): BaseNode(db) {
    registerPackageType<QH::PKG::DataPack<UsersCards>>();
    registerPackageType<QH::PKG::DataPack<Card>>();

    _timer = new QTimer(this);

    connect(_timer, &QTimer::timeout, this, &Visitor::handleTick);

}

bool Visitor::checkCardData(long long session,
                            const QString &domain, int port) {
    if (!session)
        return false;

    _lastRequested = session;

    int currentTime = time(0);
    if (_lastRequest + _requestInterval > currentTime) {

        _domain = domain;
        _port = port;
        _timer->start((_lastRequest + _requestInterval - currentTime) * 1000);

        return true;
    }

    return addNode(domain, port);
}


void Visitor::nodeConnected(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConnected(node);

    CardStatusRequest request;

    auto senderInfo = static_cast<NodeInfo*>(node);

    long long token = rand() * rand();

    senderInfo->setToken(token);

    request.setSessionId(_lastRequested);
    request.setRequestToken(token);

    sendData(&request, node->networkAddress());

    _lastRequest = time(0);
}

void Visitor::handleTick() {
    _timer->stop();
    addNode(_domain, _port);
}

int Visitor::getRequestInterval() const {
    return _requestInterval;
}

void Visitor::setRequestInterval(int newRequestInterval) {
    _requestInterval = newRequestInterval;
}

}
