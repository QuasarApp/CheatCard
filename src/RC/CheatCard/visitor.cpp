//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "CheatCard/nodeinfo.h"
#include "visitor.h"
#include <cstring>

#include "CheatCard/api/api0/cardstatusrequest.h"
#include <CheatCard/api/api0/user.h>
#include <CheatCard/api/api0/session.h>
#include <CheatCard/api/api0/userscards.h>
#include <CheatCard/api/api0/card.h>

#include <CheatCard/api/api1/restoredatarequest.h>
#include <CheatCard/api/api1/userscardsv1.h>

namespace RC {


Visitor::Visitor(QH::ISqlDBCache *db): BaseNode(db) {
    registerPackageType<QH::PKG::DataPack<UsersCards>>();
    registerPackageType<QH::PKG::DataPack<UsersCardsV1>>();
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

    auto action = [this](QH::AbstractNodeInfo *node) {
        this->action(node);
    };

    return addNode(QH::HostAddress{_domain, _port},
                   action, QH::NodeCoonectionStatus::Confirmed);
}

bool Visitor::cardValidation(const QSharedPointer<Card> &card,
                             const QByteArray &ownerSecret) const {
    Q_UNUSED(card);
    Q_UNUSED(ownerSecret);

    return true;
}

void Visitor::getSignData(QByteArray &) const {

}

bool Visitor::restoreOldData(unsigned int curentUserId,
                             const QString &domain, int port) {

    RestoreDataRequest request;
    request.setUserId(curentUserId);

    auto action = [this](QH::AbstractNodeInfo *node) {
        this->action(node);
    };

    return addNode(QH::HostAddress{domain, port},
                   action, QH::NodeCoonectionStatus::Confirmed);
}

void Visitor::nodeConnected(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConnected(node);
}

void Visitor::nodeConfirmend(QH::AbstractNodeInfo *node) {
    BaseNode::nodeConfirmend(node);
}

void Visitor::handleTick() {
    _timer->stop();
    addNode(_domain, _port);
}

void Visitor::action(QH::AbstractNodeInfo *node) {
    CardStatusRequest request;

    auto senderInfo = static_cast<NodeInfo*>(node);

    long long token = rand() * rand();

    senderInfo->setToken(token);

    request.setSessionId(_lastRequested);
    request.setRequestToken(token);

    sendData(&request, node->networkAddress());

    _lastRequest = time(0);
}

int Visitor::getRequestInterval() const {
    return _requestInterval;
}

void Visitor::setRequestInterval(int newRequestInterval) {
    _requestInterval = newRequestInterval;
}

}
