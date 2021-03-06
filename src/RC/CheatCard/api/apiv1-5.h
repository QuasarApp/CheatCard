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
#include <Private/packadata.h>

namespace RC {

namespace API {
class Contacts;
}

namespace APIv1_5 {
class ChangeUsersCards;
class StatusAfterChanges;
class CardUpdated;
class UpdateContactData;
class UpdateContactDataResponce;
class RestoreResponce;
class UsersCards;
class UsersCards;
class DeleteCardRequest;
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
    void sendSessions(const QHash<long long, QSharedPointer<API::Session> > &sessions,
                      QH::AbstractNodeInfo *dist) override;
    bool sendContacts(const API::Contacts& conntact,
                      const QByteArray& secreet,
                      bool removeRequest,
                      QH::AbstractNodeInfo *dist);

    bool deleteCard(unsigned int cardId, QH::AbstractNodeInfo *dist);

    bool sendUpdateCard(unsigned int cardId, unsigned int version, QH::AbstractNodeInfo *dist);
protected:
    IAPIObjectsFactory *initObjectFactory() const override;
    bool processCardStatusRequest(const QSharedPointer<API::CardStatusRequest> &message,
                                  const QH::AbstractNodeInfo *sender, const QH::Header&) ;

    bool processChanges(const QSharedPointer<APIv1_5::ChangeUsersCards> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&) ;

    bool processSession(const QSharedPointer<API::Session> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&);

    bool processRestoreResponce(const QSharedPointer<APIv1_5::RestoreResponce> &message,
                                const QH::AbstractNodeInfo *sender, const QH::Header&hdr);

    bool processContacts(const QSharedPointer<APIv1_5::UpdateContactData> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&);

    bool processContactsResponce(const QSharedPointer<APIv1_5::UpdateContactDataResponce> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&);

    bool processCardStatus(const QSharedPointer<QH::PKG::DataPack<APIv1_5::UsersCards>> &cardStatuses,
                           const QH::AbstractNodeInfo *sender, const QH::Header&pkg);

    void processCardStatusWithoutCardRequests(
            const QSharedPointer<QH::PKG::DataPack<APIv1_5::UsersCards>> &cardStatuses);


    bool processCardRequest(const QSharedPointer<API::CardDataRequest> &cardStatus,
                            const QH::AbstractNodeInfo *sender, const QH::Header&);

    bool processCardData(const QSharedPointer<QH::PKG::DataPack<APIv1::Card> > &cardrequest,
                         const QH::AbstractNodeInfo *sender, const QH::Header &) ;

    bool processCardUpdate(const QSharedPointer<APIv1_5::CardUpdated> &cardrequest,
                         const QH::AbstractNodeInfo *sender, const QH::Header &) ;

    bool processRestoreDataRequest(const QSharedPointer<APIv1::RestoreDataRequest> &cardrequest,
                                   const QH::AbstractNodeInfo *sender, const QH::Header &) override;

    bool processStatusAfterChanged(const QSharedPointer<APIv1_5::StatusAfterChanges> &cardrequest,
                                   const QH::AbstractNodeInfo *sender, const QH::Header &);

    bool processCardStatusImpl(const QH::PKG::DataPack<APIv1_5::UsersCards> &cardStatuses,
                               const QH::AbstractNodeInfo *sender, const QH::Header &pkg);

    bool processDeleteCardRequest(const QSharedPointer<APIv1_5::DeleteCardRequest>& request,
                                  const QH::AbstractNodeInfo *sender, const QH::Header &pkg);

    /**
     * @brief accessValidation This method must check card data only on server. This implementation do nothing.
     * @return true if card is pass validation.
     */
    bool accessValidation(const QSharedPointer<API::Card>& card,
                          const QByteArray &ownerSecret,
                          bool allowWorkers) const;
private:
    void collectDataOfuser(const QByteArray &userKey, QH::PKG::DataPack<APIv1_5::UsersCards> &responce);

    void processContactsResponcePrivate(unsigned int requestId, bool result);
    void applayUpdateContactData(const QSharedPointer<APIv1_5::UpdateContactData> &data,
                                 bool success);

    QH::PKG::DataPack<APIv1_5::UsersCards>
    lastUserStatus(unsigned int cardId);

    bool processCardStatusBase(const QSharedPointer<APIv1_5::UsersCards> &cardStatus,
                               const QByteArray &userSecreet,
                               const QH::AbstractNodeInfo *sender,
                               const QH::Header &pkg,
                               unsigned int& neededCardId);

    QSet<unsigned int> _checkUserRequestHash;
    QHash<unsigned int, QSharedPointer<RC::APIv1_5::UpdateContactData>> _waitResponce ;

};

}
#endif // APIV1_H
