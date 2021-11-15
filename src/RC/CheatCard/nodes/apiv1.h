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

/**
 * @brief The ApiV1 class
 * General changes betwin BaseNode and BaseNodev1
 *
 * The BaseNodeV1 dropped all tokens validation. now validation works only on server (used seller secret)
 */
class CHEATCARD_CORE_EXPORT ApiV1: public ApiV0
{
    Q_OBJECT
public:
    ApiV1(QH::ISqlDBCache *db);

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

    /**
     * @brief cardValidation This method must check card data only on server. This implementation do nothing.
     * @return true if card is pass validation.
     */
    virtual bool cardValidation(const QSharedPointer<Card>& card, const QByteArray &ownerSecret) const;

    /**
     * @brief sealValidation This method check seal data. This validation must be work only on server so this method implementation do nothing.
     * @return true if seal is pass validation.
     */
    virtual bool sealValidation(const QSharedPointer<UsersCards> &userCardData,
                                const QSharedPointer<Card> &cardFromDb,
                                const QByteArray &ownerSecret) const;
};
}
#endif // APIV1_H
