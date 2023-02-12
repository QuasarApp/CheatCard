//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "basenode.h"

#include <getsinglevalue.h>
#include <cmath>
#include <dbobjectsrequest.h>
#include <QCryptographicHash>

namespace RC {

BaseNode::BaseNode(const QSharedPointer<Interfaces::iDB>& db) {
    _db = db;

    setIgnoreSslErrors(QList<QSslError>() << QSslError::SelfSignedCertificate
                                          << QSslError::SelfSignedCertificateInChain
                                          << QSslError::HostNameMismatch
                                          << QSslError::CertificateUntrusted);
}

void BaseNode::nodeErrorOccured(QH::AbstractNodeInfo *nodeInfo,
                                QAbstractSocket::SocketError errorCode,
                                QString errorString) {

    QH::AbstractNode::nodeErrorOccured(nodeInfo, errorCode, errorString);

    if (QAbstractSocket::SocketError::SslInternalError == errorCode) {
        // see handleSslErrorOcurred
        return;
    }

    emit sigNetworkError(errorCode, QSslError::NoError);
}

void BaseNode::handleSslErrorOcurred(QH::SslSocket *scket, const QSslError &error) {
    QH::AbstractNode::handleSslErrorOcurred(scket, error);
    emit sigNetworkError(QAbstractSocket::SocketError::SslInternalError,
                         error.error());
}

QString BaseNode::libVersion() {
    return CHEAT_CARD_VERSION;
}

const QSharedPointer<Interfaces::iDB> &BaseNode::db() const {
    return _db;
}

void BaseNode::init() {
    initCheatCardBaseResources();
}

}
