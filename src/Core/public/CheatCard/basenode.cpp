//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "basenode.h"

#include "CheatCard/nodeinfo.h"
#include "api/apibase.h"
#include <getsinglevalue.h>
#include <cmath>
#include <dbobjectsrequest.h>
#include <settings.h>
#include <QCryptographicHash>

namespace RC {

BaseNode::BaseNode(const QSharedPointer<Interfaces::iDB>& db) {
    _db = db;

    setIgnoreSslErrors(QList<QSslError>() << QSslError::SelfSignedCertificate
                                          << QSslError::SelfSignedCertificateInChain
                                          << QSslError::HostNameMismatch
                                          << QSslError::CertificateUntrusted);
}

QH::AbstractNodeInfo *BaseNode::createNodeInfo(QAbstractSocket *socket,
                                               const QH::HostAddress *clientAddress) const {
    return new NodeInfo(socket, clientAddress);
}

void BaseNode::nodeConnected(QH::AbstractNodeInfo *node) {

    QH::AbstractNode::nodeConnected(node);
    emit sigDataExchangingChanged(connectionsCount());
}

void BaseNode::nodeDisconnected(QH::AbstractNodeInfo * node) {
    QH::AbstractNode::nodeDisconnected(node);
    emit sigDataExchangingChanged(connectionsCount());
}

void BaseNode::addNodeFailed(QH::AddNodeError) {
    setFNetAvailable(false);
}

void BaseNode::nodeAddedSucessful(QH::AbstractNodeInfo *) {
    setFNetAvailable(true);
}

void BaseNode::nodeErrorOccured(QH::AbstractNodeInfo *nodeInfo,
                                QAbstractSocket::SocketError errorCode,
                                QString errorString) {

    QH::AbstractNode::nodeErrorOccured(nodeInfo, errorCode, errorString);

    if (QAbstractSocket::SocketError::SslInternalError == errorCode) {
        // see handleSslErrorOcurred
        return;
    }

    setFNetAvailable(false);

    emit sigNetworkError(errorCode, QSslError::NoError);
}

void BaseNode::handleSslErrorOcurred(QH::SslSocket *scket, const QSslError &error) {
    QH::AbstractNode::handleSslErrorOcurred(scket, error);
    emit sigNetworkError(QAbstractSocket::SocketError::SslInternalError,
                         error.error());
}

bool BaseNode::fNetAvailable() const {
    return _fNetAvailable;
}

void BaseNode::checkNetworkConnection(const QString &domain, int port) {
    auto action = [this](QH::AbstractNodeInfo *node) {
        removeNode(node);
    };

    if (domain.isEmpty()) {
        addNode(getServerHost(), port, action,
                QH::NodeCoonectionStatus::Confirmed);
        return;
    }

    addNode(domain, port, action,
            QH::NodeCoonectionStatus::Confirmed);
}

QSharedPointer<Interfaces::iAPI> BaseNode::getApi(QH::AbstractNodeInfo *node) {
    return selectParser(API_BASE_PARSE_IS, node).dynamicCast<RC::Interfaces::iAPI>();
}

void BaseNode::setFNetAvailable(bool newFNetAvailable) {
    if (_fNetAvailable == newFNetAvailable) {
        return;
    }

    _fNetAvailable = newFNetAvailable;

    emit sigAvailableNetworkChanged(_fNetAvailable);
}

const QSharedPointer<Interfaces::iUser> &BaseNode::currentUser() const {
    return _currentUser;
}

void BaseNode::setCurrentUser(QSharedPointer<Interfaces::iUser> newCurrentUser) {
    _currentUser = newCurrentUser;
}

QString BaseNode::libVersion() {
    return CHEAT_CARD_VERSION;
}

bool BaseNode::restoreAllData(const QByteArray &curentUserKey,
                              const QString &domain, int port) {

    auto action = [this, curentUserKey](QH::AbstractNodeInfo *node) {
        auto api = getApi(node);
        if (api) {
            api->restoreOldDateRequest(curentUserKey, node);
        }
    };

    if (domain.isEmpty()) {
        return addNode(getServerHost(), port, action,
                       QH::NodeCoonectionStatus::Confirmed);
    }

    return addNode(domain, port, action,
                   QH::NodeCoonectionStatus::Confirmed);
}

bool BaseNode::updateContactData(const QSharedPointer<Interfaces::iContacts> &contact,
                                 const QByteArray& secreet,
                                 bool removeRequest,
                                 const QString &domain, int port) {

    auto action = [this, contact, secreet, removeRequest](QH::AbstractNodeInfo *node) {
        auto api = getApi(node);
        if (api) {
            api->sendContacts(*contact, secreet, removeRequest, node);
        }
    };

    if (domain.isEmpty()) {
        return addNode(getServerHost(), port, action,
                       QH::NodeCoonectionStatus::Confirmed);
    }

    return addNode(domain, port, action,
                   QH::NodeCoonectionStatus::Confirmed);
}

bool BaseNode::restoreOneCard(unsigned int cardId, const QString &domain, int port) {
    auto action = [this, cardId](QH::AbstractNodeInfo *node) {
        auto api = getApi(node);
        if (api) {
            api->restoreOneCardRequest(cardId, node);
        }
    };

    if (domain.isEmpty()) {
        return addNode(getServerHost(), port, action,
                       QH::NodeCoonectionStatus::Confirmed);
    }

    return addNode(domain, port, action,
                   QH::NodeCoonectionStatus::Confirmed);
}

QString BaseNode::getServerHost() const {
    auto settings = QuasarAppUtils::ISettings::instance();

    if (!settings)
        return DEFAULT_CHEAT_CARD_HOST;

    if (!settings->getValue("devSettingEnable", false).toBool()) {
        return DEFAULT_CHEAT_CARD_HOST;
    }

    return settings->getStrValue("host", DEFAULT_CHEAT_CARD_HOST);
}

const QSharedPointer<Interfaces::iDB> &BaseNode::db() const {
    return _db;
}

void BaseNode::init() {
    initCheatCardBaseResources();
}

bool BaseNode::createContact(const QSharedPointer<Interfaces::iUser> &anotherUser,
                             QSharedPointer<Interfaces::iContacts> &resultContact) {

    if (!_currentUser->isValid())
        return false;

    if (_currentUser->id() == anotherUser->id()) {
        return false;
    }

    resultContact->setUserKey(_currentUser->getKey());
    resultContact->setInfo(anotherUser->name());
    resultContact->setChildUserKey(anotherUser->getKey());

    return resultContact->isValid();
}
}
