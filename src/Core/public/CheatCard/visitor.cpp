//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "visitor.h"
#include <cstring>
#include "api/apibase.h"


namespace RC {


Visitor::Visitor(const QSharedPointer<Interfaces::iDB> &db): BaseNode(db) {

    _timer = new QTimer(this);

    connect(_timer, &QTimer::timeout, this, &Visitor::handleTick);

}

bool Visitor::sendRequestPrivate() {
    auto action = [this](QH::AbstractNodeInfo *node) {
        auto api = getApi(node);

        if (!api)
            return;

        api->sendCardStatusRequest(_lastRequested, node);
        _lastRequest = time(0);
    };

    return addNode(_domain, _port,
                   action, QH::NodeCoonectionStatus::Confirmed);
}

bool Visitor::checkCardData(long long session,
                            const QString &domain, int port) {
    if (!session)
        return false;

    _lastRequested = session;

    int currentTime = time(0);

    if (domain.isEmpty()) {
        _domain = getServerHost();
    } else {
        _domain = domain;
    }
    _port = port;


    if (_lastRequest + _requestInterval > currentTime) {

        _timer->start((_lastRequest + _requestInterval - currentTime) * 1000);
        return true;
    }

    return sendRequestPrivate();
}

BaseNode::NodeType Visitor::nodeType() const {
    return static_cast<BaseNode::NodeType>(API::APIBase::NodeType::Visitor);
}

void Visitor::nodeConnected(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConnected(node);
}

void Visitor::nodeConfirmend(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConfirmend(node);
}

void Visitor::handleTick() {
    _timer->stop();
    sendRequestPrivate();
}

int Visitor::getRequestInterval() const {
    return _requestInterval;
}

void Visitor::setRequestInterval(int newRequestInterval) {
    _requestInterval = newRequestInterval;
}

}
