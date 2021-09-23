//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#
#include "node.h"

#include <QSharedPointer>
#include <QTimer>
#include <userscards.h>

namespace RC {

Node::Node(QH::ISqlDBCache *db) {
    _db = db;
}

QSharedPointer<UsersCards>
Node::getUserCardData(unsigned int userId, unsigned int cardId) {
    UsersCards request;
    request.setCard(cardId);
    request.setUser(userId);

    auto purches = _db->getObject(request);
    return purches;
}

NodeStatus Node::lastStatus() const {
    return _lastStatus;
}

void Node::setLastStatus(NodeStatus newLastStatus) {
    _lastStatus = newLastStatus;
}

Error Node::lastError() const {
    return _lastError;
}

void Node::setLastError(Error newLastError) {
    _lastError = newLastError;
}

int Node::getPurchasesCount(unsigned int userId, unsigned int cardId) {

    auto purches = getUserCardData(userId, cardId);

    if (purches) {
        return purches->getPurchasesNumber();
    }

    return 0;
}

void Node::beginWork() {
    _lastStatus = NodeStatus::InProgress;
    emit sigSessionWasBegin();
}

void Node::endWork(Error status) {
    _lastError = status;

    emit sigSessionWasFinshed(status);
}
}
