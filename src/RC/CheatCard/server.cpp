//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "CheatCard/carddatarequest.h"
#include "CheatCard/cardstatusrequest.h"
#include "server.h"
#include "user.h"

#include <CheatCard/card.h>
#include <CheatCard/session.h>
#include <CheatCard/userscards.h>

#include <badrequest.h>
#include "CheatCard/clearolddata.h"
#include <QCoreApplication>

namespace RC {

Server::Server(QH::ISqlDBCache *db): BaseNode(db) {
    registerPackageType<Session>();
    registerPackageType<CardStatusRequest>();
    registerPackageType<QH::PKG::DataPack<UsersCards>>();
    registerPackageType<CardDataRequest>();
    registerPackageType<QH::PKG::DataPack<Card>>();

    auto task = QSharedPointer<ClearOldData>::create();
    task->setTime(0);
    task->setMode(QH::ScheduleMode::SingleWork);

    sheduleTask(task);

    task = QSharedPointer<ClearOldData>::create();
    task->setTime(30 * ClearOldData::Day);
    task->setMode(QH::ScheduleMode::Repeat);

    sheduleTask(task);
}

bool Server::cardValidation(const QSharedPointer<Card> &card,
                            const QByteArray &ownerSecret) const {

    return card && card->ownerSignature() == User::makeKey(ownerSecret);
}

void Server::nodeConnected(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConnected(node);
    auto address = node->networkAddress();
    QTimer::singleShot(WAIT_CONFIRM_TIME, this, [this, address](){
        removeNode(address);
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

QH::ParserResult Server::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                      const QH::Header &pkgHeader,
                                      const QH::AbstractNodeInfo *sender) {
    QH::ParserResult result = BaseNode::parsePackage(pkg, pkgHeader, sender);

    if (result == QH::ParserResult::Error) {
        removeNode(sender->networkAddress());
    }

    return result;
}

}
