//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "CheatCard/carddatarequest.h"
#include "CheatCard/cardstatusrequest.h"
#include "serverv1.h"

#include <CheatCard/card.h>
#include <CheatCard/session.h>
#include <CheatCard/userscards.h>

#include <badrequest.h>
#include "CheatCard/clearolddata.h"
#include <QCoreApplication>

namespace RC {

ServerV1::ServerV1(QH::ISqlDBCache *db): Server(db) {
    QH::SslSrtData sslData;
    sslData.commonName = DEFAULT_CHEAT_CARD_HOST;
    sslData.organization = QCoreApplication::organizationName();

    useSelfSignedSslConfiguration(sslData);
}

void ServerV1::nodeConnected(QH::AbstractNodeInfo *node) {
    Server::nodeConnected(node);
}

void ServerV1::nodeDisconnected(QH::AbstractNodeInfo *node) {
    Server::nodeDisconnected(node);
}

void ServerV1::nodeErrorOccured(QH::AbstractNodeInfo *nodeInfo,
                                QAbstractSocket::SocketError errorCode,
                                QString errorString) {
    Server::nodeErrorOccured(nodeInfo, errorCode, errorString);

}

bool ServerV1::processCardStatusRequest(const QSharedPointer<CardStatusRequest> &message,
                                      const QH::AbstractNodeInfo *sender,
                                      const QH::Header &hdr) {

    return Server::processCardStatusRequest(message, sender, hdr);

}

bool ServerV1::processSession(const QSharedPointer<Session> &message,
                            const QH::AbstractNodeInfo *sender,
                            const QH::Header &hdr) {

    return Server::processSession(message, sender, hdr);

}

bool ServerV1::processCardStatus(const QSharedPointer<QH::PKG::DataPack<UsersCards> > &cardStatuses,
                               const QH::AbstractNodeInfo *sender,
                               const QH::Header &hdr) {

    return Server::processCardStatus(cardStatuses, sender, hdr);

}

bool ServerV1::applayPurchases(const QSharedPointer<UsersCards> &dbCard,
                             const QH::AbstractNodeInfo *sender) {

    return Server::applayPurchases(dbCard, sender);

}

bool ServerV1::processCardRequest(const QSharedPointer<CardDataRequest> &cardStatus,
                                const QH::AbstractNodeInfo *sender,
                                const QH::Header &hdr) {

    return Server::processCardRequest(cardStatus, sender, hdr);

}

bool ServerV1::processCardData(const QSharedPointer<QH::PKG::DataPack<Card> > &cardrequest,
                             const QH::AbstractNodeInfo *sender,
                             const QH::Header &hdr) {

    return Server::processCardData(cardrequest, sender, hdr);

}

QH::ParserResult ServerV1::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                      const QH::Header &pkgHeader,
                                      const QH::AbstractNodeInfo *sender) {

    return Server::parsePackage(pkg, pkgHeader, sender);
}

}
