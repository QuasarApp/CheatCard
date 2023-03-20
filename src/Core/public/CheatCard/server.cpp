//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "server.h"
#include <badrequest.h>
#include <QCoreApplication>
#include <api/apibase.h>

namespace RC {

Server::Server(const QSharedPointer<Interfaces::iDB> &db): BaseNode(db) {

}

BaseNode::NodeType Server::nodeType() const {
    return BaseNode::NodeType::Server;
}

void Server::nodeConnected(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConnected(node);

    if (auto api = selectParser(API_BASE_PARSE_IS, 3).dynamicCast<Interfaces::iAPI>()) {
        api->sendOldVersionPackage(node);
    }
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
