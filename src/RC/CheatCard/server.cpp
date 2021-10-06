//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "carddatarequest.h"
#include "cardstatusrequest.h"
#include "server.h"

#include <CheatCard/card.h>
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

void Server::nodeConnected(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConnected(node);

    QTimer::singleShot(WAIT_CONFIRM_TIME, this, [this, node](){
        removeNode(node);
    });
}

void Server::nodeDisconnected(QH::AbstractNodeInfo *node) {
    BaseNode::nodeDisconnected(node);
}

void Server::nodeErrorOccured(QH::AbstractNodeInfo *nodeInfo, QAbstractSocket::SocketError errorCode, QString errorString) {
    // ignore QAbstractSocket::SocketError::RemoteHostClosedError
    if (errorCode != QAbstractSocket::SocketError::RemoteHostClosedError) {
        BaseNode::nodeErrorOccured(nodeInfo, errorCode, errorString);
    }
}

}
