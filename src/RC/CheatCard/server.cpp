//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "itargetnode.h"
#include "server.h"

namespace RC {

Server::Server(DB *db): Node(db)
{

}

void Server::connectionReceived(ITargetNode *obj) {

    if (_currentTarget) {
        disconnect(_currentTarget.data(), &ITargetNode::sigMessageReceived,
                this, &IConnectorBackEnd::handleReceiveMessage);

        disconnect(_currentTarget.data(), &ITargetNode::sigConnectionClosed,
                this, &IConnectorBackEnd::handleConnectionClosed);
    }

    _currentTarget = QSharedPointer<ITargetNode>(obj);

    if (_currentTarget) {
        connect(_currentTarget.data(), &ITargetNode::sigMessageReceived,
                this, &IConnectorBackEnd::handleReceiveMessage, Qt::QueuedConnection);
        connect(_currentTarget.data(), &ITargetNode::sigConnectionClosed,
                this, &IConnectorBackEnd::handleConnectionClosed, Qt::QueuedConnection);

    }

    if (mode() == Client) {

        UserHeader request;
        request.userId = _activeUser->userId();
        request.command = UserId;

        std::memcpy(request.token, _activeUser->getKey().data(), sizeof(request.token));

        QByteArray data = QByteArray::fromRawData(reinterpret_cast<char*>(&request), sizeof(request));

        if (!_currentTarget->sendMessage(data)) {
            QuasarAppUtils::Params::log("Failed to send responce", QuasarAppUtils::Error);
            return;
        }
    }

    beginWork();
}

void Server::connectionLost(unsigned int nodeID) {
    if (_currentTarget->nodeId() != nodeID) {
        QuasarAppUtils::Params::log("Try drop another connection!!", QuasarAppUtils::Error);
    }

    endWork(ConnectionLost);
}
}
