//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef APIV3_H
#define APIV3_H
#include "abstractnode.h"
#include "datapack.h"
#include <isqldb.h>
#include <api/apibase.h>

namespace RC {

namespace API {

namespace V3 {
class ChangeUsersCards;
class StatusAfterChanges;
class CardUpdated;
class UpdateContactData;
class UpdateContactDataResponce;
class RestoreResponce;
class DeleteCardRequest;
}


/**
 * @brief The ApiV3 class Is a first independet api class of older api versions.
 *
 * Main changes betveen apiv2
 * - All clients nodes has infinity connections with a server.
 * - Client diskonect from the server only when is suspend or exists.
 * - Disable using short ids of the clients. Only publick sha256 keys.
 *
 */
class ApiV3: public APIBase
{
    Q_OBJECT
public:
    ApiV3(const QSharedPointer<Interfaces::iDB>& dataBase, QH::AbstractNode* mainNode);
    void initSupportedCommands() override;

    int version() const override;
    QH::ParserResult parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                  const QH::Header &pkgHeader,
                                  QH::AbstractNodeInfo *sender) override;

    void sendCardStatusRequest(long long userSession, QH::AbstractNodeInfo *dist) override;

    void restoreOldDateRequest(const QByteArray &curentUserKey, QH::AbstractNodeInfo *dist) override;
    void restoreOneCardRequest(unsigned int cardId, QH::AbstractNodeInfo *dist) override;
    void sendSessions(const QHash<long long, QSharedPointer<Interfaces::iSession> > &sessions,
                      QH::AbstractNodeInfo *dist) override;
    bool sendContacts(const Interfaces::iContacts& conntact,
                      const QByteArray& secreet,
                      bool removeRequest,
                      QH::AbstractNodeInfo *dist) override;

    bool deleteCard(unsigned int cardId,
                    const QByteArray &curentUserKey,
                    QH::AbstractNodeInfo *dist) override;

    bool sendUpdateCard(unsigned int cardId, unsigned int version, QH::AbstractNodeInfo *dist) override;
    bool changeUsersData(const QByteArray &sellerUserKey, unsigned int cardId,
                         unsigned int userId,
                         unsigned long long session,
                         unsigned int purchasesCount,
                         unsigned int receivedCount,
                         QH::AbstractNodeInfo *dist) override;
protected:
    bool processCardStatusRequest(const QSharedPointer<V3::CardStatusRequest> &message,
                                  const QH::AbstractNodeInfo *sender, const QH::Header&) ;

    bool processChanges(const QSharedPointer<V3::ChangeUsersCards> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&) ;

    bool processSession(const QSharedPointer<V3::Session> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&);

    bool processRestoreResponce(const QSharedPointer<V3::RestoreResponce> &message,
                                const QH::AbstractNodeInfo *sender, const QH::Header&hdr);

    bool processContacts(const QSharedPointer<V3::UpdateContactData> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&);

    bool processContactsResponce(const QSharedPointer<V3::UpdateContactDataResponce> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&);

    bool processCardStatus(const QSharedPointer<QH::PKG::DataPack<V3::UsersCards>> &cardStatuses,
                           const QH::AbstractNodeInfo *sender, const QH::Header&pkg);

    void processCardStatusWithoutCardRequests(
            const QSharedPointer<QH::PKG::DataPack<V3::UsersCards>> &cardStatuses);


    bool processCardRequest(const QSharedPointer<V3::CardDataRequest> &cardStatus,
                            const QH::AbstractNodeInfo *sender, const QH::Header&);

    bool processCardData(const QSharedPointer<QH::PKG::DataPack<V3::Card> > &cardrequest,
                         const QH::AbstractNodeInfo *sender, const QH::Header &) ;

    bool processCardUpdate(const QSharedPointer<V3::CardUpdated> &cardrequest,
                         const QH::AbstractNodeInfo *sender, const QH::Header &) ;

    bool processRestoreDataRequest(const QSharedPointer<V3::RestoreDataRequest> &cardrequest,
                                   const QH::AbstractNodeInfo *sender, const QH::Header &) override;

    bool processStatusAfterChanged(const QSharedPointer<V3::StatusAfterChanges> &cardrequest,
                                   const QH::AbstractNodeInfo *sender, const QH::Header &);

    bool processCardStatusImpl(const QH::PKG::DataPack<V3::UsersCards> &cardStatuses,
                               const QH::AbstractNodeInfo *sender, const QH::Header &pkg);

    bool processDeleteCardRequest(const QSharedPointer<V3::DeleteCardRequest>& request,
                                  const QH::AbstractNodeInfo *sender, const QH::Header &pkg);

    /**
     * @brief accessValidation This method must check card data only on server. This implementation do nothing.
     * @return true if card is pass validation.
     */
    bool accessValidation(const QSharedPointer<Interfaces::iCard> &card,
                          const QByteArray &ownerSecret,
                          bool allowWorkers) const;


private:
    void collectDataOfuser(const QByteArray &userKey, QH::PKG::DataPack<RC::API::V2::UsersCards> &responce);

    void processContactsResponcePrivate(unsigned int requestId, bool result);
    void applayUpdateContactData(const QSharedPointer<V3::UpdateContactData> &data,
                                 bool success);

    QH::PKG::DataPack<V3::UsersCards>
    lastUserStatus(unsigned int cardId);

    bool processCardStatusBase(const QSharedPointer<V3::UsersCards> &cardStatus,
                               const QByteArray &userSecreet,
                               const QH::AbstractNodeInfo *sender,
                               const QH::Header &pkg,
                               unsigned int& neededCardId);

    QSet<unsigned int> _checkUserRequestHash;
    QHash<unsigned int, QSharedPointer<V3::UpdateContactData>> _waitResponce ;

};
}
}
#endif // APIV2_H
