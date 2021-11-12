//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SERVERV1_H
#define SERVERV1_H

#include "abstractnode.h"
#include "basenode.h"
#include "server.h"
#include "CheatCard/clearolddata.h"

#include <isqldbcache.h>

namespace RC {

class CHEATCARD_CORE_EXPORT ServerV1: public Server
{
    Q_OBJECT
public:
    ServerV1(QH::ISqlDBCache *db);

    // AbstractNode interface
protected:
    void nodeConnected(QH::AbstractNodeInfo *node) override;
    void nodeDisconnected(QH::AbstractNodeInfo *node) override;
    void nodeErrorOccured(QH::AbstractNodeInfo *nodeInfo,
                          QAbstractSocket::SocketError errorCode,
                          QString errorString) override;

    bool processCardStatusRequest(const QSharedPointer<CardStatusRequest> &message,
                                  const QH::AbstractNodeInfo *sender, const QH::Header&) override;

    bool processSession(const QSharedPointer<Session> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&) override;
    bool processCardStatus(const QSharedPointer<QH::PKG::DataPack<UsersCards>> &cardStatuses,
                           const QH::AbstractNodeInfo *sender, const QH::Header&) override;
    bool applayPurchases(const QSharedPointer<UsersCards> &dbCard,
                         const QH::AbstractNodeInfo *sender) override;
    bool processCardRequest(const QSharedPointer<CardDataRequest> &cardStatus,
                            const QH::AbstractNodeInfo *sender, const QH::Header&) override;
    bool processCardData(const QSharedPointer<QH::PKG::DataPack<Card> > &cardrequest,
                         const QH::AbstractNodeInfo *sender, const QH::Header &) override;

    QH::ParserResult parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                  const QH::Header &pkgHeader,
                                  const QH::AbstractNodeInfo *sender) override;

    bool cardValidation(const QSharedPointer<Card>& card,
                        const QByteArray &ownerSecret) const override;

    bool sealValidation(const QSharedPointer<UsersCards> &userCardData,
                                const QSharedPointer<Card> &cardFromDb,
                                const QByteArray &ownerSecret) const override;

    friend class ClearOldData;


};
}
#endif // SERVERV1_H
