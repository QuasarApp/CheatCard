//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "server.h"

#include "CheatCard/api/api0/user.h"
#include <CheatCard/api/api0/card.h>
#include <CheatCard/api/api0/session.h>
#include <CheatCard/api/api0/userscards.h>
#include "CheatCard/api/api0/carddatarequest.h"
#include "CheatCard/api/api0/cardstatusrequest.h"

#include <CheatCard/api/api1/restoredatarequest.h>
#include <CheatCard/api/api1/userscardsv1.h>

#include <badrequest.h>
#include "CheatCard/clearolddata.h"
#include <QCoreApplication>

namespace RC {

Server::Server(QH::ISqlDBCache *db): BaseNode(db) {
    registerPackageType<Session>();
    registerPackageType<CardStatusRequest>();
    registerPackageType<QH::PKG::DataPack<UsersCards>>();
    registerPackageType<QH::PKG::DataPack<UsersCardsV1>>();

    registerPackageType<CardDataRequest>();
    registerPackageType<QH::PKG::DataPack<Card>>();
    registerPackageType<RestoreDataRequest>();

    auto task = QSharedPointer<ClearOldData>::create();
    task->setTime(0);
    task->setMode(QH::ScheduleMode::SingleWork);

    sheduleTask(task);

    task = QSharedPointer<ClearOldData>::create();
    task->setTime(30 * ClearOldData::Day);
    task->setMode(QH::ScheduleMode::Repeat);

    sheduleTask(task);
}

bool Server::cardValidation(const QSharedPointer<Card> &cardFromDB,
                            const QByteArray &ownerSecret) const {

    if (!cardFromDB)
        return true;

    auto signature = cardFromDB->ownerSignature();
    auto ownerSignature =  User::makeKey(ownerSecret);

    return signature == ownerSignature;
}

void Server::getSignData(QByteArray &) const {

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
