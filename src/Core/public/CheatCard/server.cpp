//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "server.h"
#include "api.h"
#include <badrequest.h>
#include <QCoreApplication>
#include <api/apibase.h>

namespace RC {

Server::Server(const QSharedPointer<Interfaces::iDB> &db, const QVector<unsigned short> &apiVesions): BaseNode(db) {
    API::init(apiVesions, db, this);

    QH::SslSrtData sslData;
    sslData.commonName = getServerHost();
    sslData.organization = QCoreApplication::organizationName();

    useSelfSignedSslConfiguration(sslData);
}

BaseNode::NodeType Server::nodeType() const {
    return BaseNode::NodeType::Server;
}

void Server::nodeConfirmend(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConfirmend(node);

    auto count = connectionsCount();

    if (maxCountConnections < count) {
        maxCountConnections = count;
        QuasarAppUtils::Params::log(
            QString("Detected new record of server loading! connection count = %0").arg(maxCountConnections),
            QuasarAppUtils::Info);
    }

}

void Server::nodeConnected(QH::AbstractNodeInfo *node) {

    if (auto api = selectParser(API_BASE_PARSE_IS, 3).dynamicCast<Interfaces::iAPI>()) {
        api->sendOldVersionPackage(node);
    }

    BaseNode::nodeConnected(node);
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

int Server::getMaxCountConnections() const {
    return maxCountConnections;
}

bool Server::restoreCardsWithotRootSeals() {
    const auto cards = db()->getAllCards();

    for (const auto &card: cards) {
        auto userCard = db()->makeEmptyUsersCard();
        userCard->setCard(card->cardId());
        userCard->setUser(card->ownerSignature());

        if (!db()->saveUsersCard(userCard)) {
            return false;
        }
    }

    return true;
}

}
