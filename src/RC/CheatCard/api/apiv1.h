//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef APIV1_H
#define APIV1_H
#include "abstractnode.h"
#include "CheatCard/core_global.h"
#include "apiv0.h"
#include "datapack.h"
#include <isqldbcache.h>

namespace RC {

namespace APIv1 {
class RestoreDataRequest;
class UsersCards;
class Card;
}


/**
 * @brief The ApiV1 class
 * General changes betwin BaseNode and BaseNodev1
 *
 * The BaseNodeV1 dropped all tokens validation. now validation works only on server (used seller secret)
 */
class CHEATCARD_CORE_EXPORT ApiV1: public ApiV0
{
public:
    ApiV1(BaseNode* node);

    int version() const override;
    QH::ParserResult parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                  const QH::Header &pkgHeader,
                                  const QH::AbstractNodeInfo *sender) override;

    void sendCardStatusRequest(long long userSession, QH::AbstractNodeInfo *dist) override;

protected:
    IAPIObjectsFactory *initObjectFactory() const override;
    bool processCardStatusRequest(const QSharedPointer<API::CardStatusRequest> &message,
                                  const QH::AbstractNodeInfo *sender, const QH::Header&) ;

    bool processSession(const QSharedPointer<API::Session> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&);
    bool processCardStatus(const QSharedPointer<QH::PKG::DataPack<APIv1::UsersCards>> &cardStatuses,
                           const QH::AbstractNodeInfo *sender, const QH::Header&);
    bool applayPurchases(const QSharedPointer<APIv1::UsersCards> &dbCard,
                         const QH::AbstractNodeInfo *sender) ;
    bool processCardRequest(const QSharedPointer<API::CardDataRequest> &cardStatus,
                            const QH::AbstractNodeInfo *sender, const QH::Header&) ;
    bool processCardData(const QSharedPointer<QH::PKG::DataPack<APIv1::Card> > &cardrequest,
                         const QH::AbstractNodeInfo *sender, const QH::Header &) ;

    virtual bool processRestoreDataRequest(const QSharedPointer<APIv1::RestoreDataRequest> &cardrequest,
                                           const QH::AbstractNodeInfo *sender, const QH::Header &);



};
}
#endif // APIV1_H
