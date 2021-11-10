//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "basenodev1.h"
#include "CheatCard/carddatarequest.h"
#include "CheatCard/cardstatusrequest.h"
#include "CheatCard/nodeinfo.h"
#include "CheatCard/user.h"

#include <CheatCard/card.h>
#include <dbobjectsrequest.h>
#include <CheatCard/session.h>
#include <CheatCard/userscards.h>
#include <getsinglevalue.h>
#include <cmath>
#include <dbobjectsrequest.h>

namespace RC {

BaseNodeV1::BaseNodeV1(QH::ISqlDBCache *db): BaseNode(db) {

    useSystemSslConfiguration();
    setIgnoreSslErrors(QList<QSslError>() << QSslError::SelfSignedCertificate
                       << QSslError::SelfSignedCertificateInChain
                       << QSslError::HostNameMismatch);
}

QH::ParserResult BaseNodeV1::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                        const QH::Header &pkgHeader,
                                        const QH::AbstractNodeInfo *sender) {

    auto parentResult = BaseNode::parsePackage(pkg, pkgHeader, sender);
    if (parentResult != QH::ParserResult::NotProcessed) {
        return parentResult;
    }

    return QH::ParserResult::NotProcessed;
}

bool BaseNodeV1::processCardStatus(const QSharedPointer<QH::PKG::DataPack<UsersCards> > &cardStatuses,
                                 const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    return BaseNode::processCardStatus(cardStatuses, sender, hdr);
}

bool BaseNodeV1::applayPurchases(const QSharedPointer<UsersCards> &dbCard,
                               const QH::AbstractNodeInfo * sender) {

    return BaseNode::applayPurchases(dbCard, sender);
}

bool BaseNodeV1::processCardRequest(const QSharedPointer<CardDataRequest> &cardrequest,
                                  const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    return BaseNode::processCardRequest(cardrequest, sender, hdr);
}

bool BaseNodeV1::processCardData(const QSharedPointer<QH::PKG::DataPack<Card>> &cards,
                               const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    return BaseNode::processCardData(cards, sender, hdr);
}

bool BaseNodeV1::processCardStatusRequest(const QSharedPointer<CardStatusRequest> &cardStatus,
                                        const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    return BaseNode::processCardStatusRequest(cardStatus, sender, hdr);
}

bool BaseNodeV1::processSession(const QSharedPointer<Session> &session,
                              const QH::AbstractNodeInfo *sender, const QH::Header &hdr) {

    return BaseNode::processSession(session, sender, hdr);

}


}
