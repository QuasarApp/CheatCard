//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef APIV1_H
#define APIV1_H

#include "api1/restoredatarequest.h"
#include "api1/userscards.h"
#include <api1/card.h>

#include "apiv0.h"
#include "datapack.h"
#include <isqldb.h>

namespace RC {

namespace API {

/**
 * @brief The ApiV1 class
 * General changes betwin BaseNode and BaseNodev1
 *
 * The BaseNodeV1 dropped all tokens validation. now validation works only on server (used seller secret)
 */
class ApiV1: public API::ApiV0
{
    Q_OBJECT
public:
    ApiV1(const QSharedPointer<Interfaces::iDB>& dataBase, QH::AbstractNode* mainNode);
    void initSupportedCommands() override;

    int version() const override;
    QH::ParserResult parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                  const QH::Header &pkgHeader,
                                  QH::AbstractNodeInfo *sender) override;

    void sendCardStatusRequest(long long userSession, QH::AbstractNodeInfo *dist) override;

    void restoreOldDateRequest(const QByteArray &curentUserKey, QH::AbstractNodeInfo *dist) override;
    void restoreOneCardRequest(unsigned int cardId, QH::AbstractNodeInfo *dist) override;

    bool sendContacts(const Interfaces::iContacts &conntact,
                      const QByteArray &secreet,
                      bool removeRequest,
                      QH::AbstractNodeInfo *dist) override;
    bool deleteCard(unsigned int cardId,
                    QH::AbstractNodeInfo *dist) override;
    bool sendUpdateCard(unsigned int cardId,
                        unsigned int version,
                        QH::AbstractNodeInfo *dist) override;

    bool changeUsersData(const QByteArray &cardId,
                         unsigned int userId, unsigned int,
                         unsigned long long session,
                         unsigned int receivedCount, unsigned int,
                         QH::AbstractNodeInfo *dist) override;

protected:
    bool processCardStatusRequest(const QSharedPointer<API::V0::CardStatusRequest> &message,
                                  const QH::AbstractNodeInfo *sender, const QH::Header&) ;

    bool processSession(const QSharedPointer<API::V0::Session> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&);

    bool processCardStatus(const QSharedPointer<QH::PKG::DataPack<API::V1::UsersCards>> &cardStatuses,
                           const QH::AbstractNodeInfo *sender, const QH::Header&pkg);

    bool processCardStatusImpl(const QH::PKG::DataPack<API::V1::UsersCards> &cardStatuses,
                               const QH::AbstractNodeInfo *sender, const QH::Header&pkg);

    bool processCardRequest(const QSharedPointer<API::V0::CardDataRequest> &cardStatus,
                            const QH::AbstractNodeInfo *sender, const QH::Header&);
    bool processCardData(const QSharedPointer<QH::PKG::DataPack<API::V1::Card> > &cardrequest,
                         const QH::AbstractNodeInfo *sender, const QH::Header &) ;

    virtual bool processRestoreDataRequest(const QSharedPointer<API::V1::RestoreDataRequest> &cardrequest,
                                           const QH::AbstractNodeInfo *sender, const QH::Header &);

    /**
     * @brief cardValidation This method must check card data only on server. This implementation do nothing.
     * @return true if card is pass validation.
     */
    virtual bool cardValidation(const QSharedPointer<Interfaces::iCard> &card,
                                const QByteArray &ownerSecret) const;
    void collectDataOfuser(const QByteArray& userKey,
                           QH::PKG::DataPack<API::V1::UsersCards> &responce);
    
private:

    unsigned int _restoreDataPacakgeHash = 0;

};
}
}
#endif // APIV1_H
