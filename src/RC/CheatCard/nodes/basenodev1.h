//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef BASENODEV1_H
#define BASENODEV1_H
#include "abstractnode.h"
#include "CheatCard/core_global.h"
#include "basenode.h"
#include "datapack.h"
#include <isqldbcache.h>

namespace RC {

class CHEATCARD_CORE_EXPORT BaseNodeV1: public BaseNode
{
    Q_OBJECT
public:
    BaseNodeV1(QH::ISqlDBCache *db);

protected:
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

};
}
#endif // BASENODEV1_H
