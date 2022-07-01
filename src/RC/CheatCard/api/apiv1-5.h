//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef APIV1_5_H
#define APIV1_5_H
#include "abstractnode.h"
#include "CheatCard/core_global.h"
#include "apiv1.h"
#include "datapack.h"
#include <isqldbcache.h>

namespace RC {

namespace APIv1_5 {
class ChangeUsersCards;
class StatusAfterChanges;
class CardUpdated;
}


/**
 * @brief The ApiV1_5 class
 * General changes betwin BaseNode and BaseNodev1
 *
 * The BaseNodeV1 dropped all tokens validation. now validation works only on server (used seller secret)
 */
class CHEATCARD_CORE_EXPORT ApiV1_5: public ApiV1
{
public:
    ApiV1_5(BaseNode* node);

    int version() const override;
    QH::ParserResult parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                  const QH::Header &pkgHeader,
                                  const QH::AbstractNodeInfo *sender) override;

    void sendCardStatusRequest(long long userSession, QH::AbstractNodeInfo *dist) override;

    void restoreOldDateRequest(const QByteArray &curentUserKey, QH::AbstractNodeInfo *dist) override;
    void restoreOneCardRequest(unsigned int cardId, QH::AbstractNodeInfo *dist) override;

    bool sendUpdateCard(unsigned int cardId, unsigned int version, QH::AbstractNodeInfo *dist);
protected:
    IAPIObjectsFactory *initObjectFactory() const override;
    bool processCardStatusRequest(const QSharedPointer<API::CardStatusRequest> &message,
                                  const QH::AbstractNodeInfo *sender, const QH::Header&) ;

    bool processChanges(const QSharedPointer<APIv1_5::ChangeUsersCards> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&) ;

    bool processSession(const QSharedPointer<API::Session> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&);
    bool processCardStatus(const QSharedPointer<QH::PKG::DataPack<APIv1::UsersCards>> &cardStatuses,
                           const QH::AbstractNodeInfo *sender, const QH::Header&pkg);

    void processCardStatusWithoutCardRequests(
            const QSharedPointer<QH::PKG::DataPack<APIv1::UsersCards>> &cardStatuses);


    bool processCardRequest(const QSharedPointer<API::CardDataRequest> &cardStatus,
                            const QH::AbstractNodeInfo *sender, const QH::Header&) ;
    bool processCardData(const QSharedPointer<QH::PKG::DataPack<APIv1::Card> > &cardrequest,
                         const QH::AbstractNodeInfo *sender, const QH::Header &) ;

    bool processCardUpdate(const QSharedPointer<APIv1_5::CardUpdated> &cardrequest,
                         const QH::AbstractNodeInfo *sender, const QH::Header &) ;

    bool processRestoreDataRequest(const QSharedPointer<APIv1::RestoreDataRequest> &cardrequest,
                                   const QH::AbstractNodeInfo *sender, const QH::Header &) override;

    bool processStatusAfterChanged(const QSharedPointer<APIv1_5::StatusAfterChanges> &cardrequest,
                                   const QH::AbstractNodeInfo *sender, const QH::Header &);

private:

    QH::PKG::DataPack<APIv1::UsersCards>
    lastUserStatus(unsigned int cardId);

    bool processCardStatusBase(const QSharedPointer<APIv1::UsersCards> &cardStatus,
                               const QByteArray &userSecreet,
                               const QH::AbstractNodeInfo *sender,
                               const QH::Header &pkg,
                               unsigned int& neededCardId);

    unsigned int _restoreDataPacakgeHash = 0;

};

}
#endif // APIV1_H