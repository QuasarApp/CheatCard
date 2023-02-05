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
class CardUpdated;
class UpdateContactData;
class DeleteCardRequest;
class Sync;
class UsersCards;
class CardDataRequest;
class Card;
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

    void syncRequest(const QByteArray &curentUserKey,
                     QH::AbstractNodeInfo *dist,
                     const std::function<void(bool successful)>& = {}) override;


    bool sendContacts(const Interfaces::iContacts& conntact,
                      const QByteArray& secreet,
                      bool removeRequest,
                      QH::AbstractNodeInfo *dist,
                      const std::function<void(int err)>& = {}) override;

    bool deleteCard(const QByteArray &cardId,
                    const QByteArray &curentUserKey,
                    QH::AbstractNodeInfo *dist,
                    const std::function<void(int err)>& = {}) override;

    bool sendUpdateCard(const QByteArray &cardId,
                        unsigned int version,
                        QH::AbstractNodeInfo *dist,
                        const std::function<void(int err)>& = {}) override;

    bool changeUsersData(const QByteArray &sellerUserKey,
                         const QByteArray &cardId,
                         const QByteArray &userId,
                         unsigned int purchasesCount,
                         unsigned int receivedCount,
                         QH::AbstractNodeInfo *dist,
                         const std::function<void(int err, const QSharedPointer<Interfaces::iUsersCards>& currentState)>& = {}) override;
protected:
    bool processSync(const QSharedPointer<V3::Sync> &message,
                     const QH::AbstractNodeInfo *sender, const QH::Header&) ;

    bool processChanges(const QSharedPointer<V3::ChangeUsersCards> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&) ;

    bool processContacts(const QSharedPointer<V3::UpdateContactData> &message,
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
    void collectDataOfuser(const QByteArray &userKey,
                           QH::PKG::DataPack<RC::API::V3::UsersCards> &responce);

    bool applayPurchases(const QSharedPointer<API::V3::UsersCards> &dbCard,
                         const QH::AbstractNodeInfo *sender, bool alert = true);

    QH::PKG::DataPack<API::V3::UsersCards>
    lastUserStatus(const QByteArray &cardId);


    bool processCardStatusBase(const QSharedPointer<V3::UsersCards> &cardStatus,
                               const QByteArray &userSecreet,
                               const QH::AbstractNodeInfo *sender,
                               const QH::Header &pkg,
                               QByteArray &neededCardId);

    bool cardValidation(const QSharedPointer<RC::Interfaces::iCard> &cardFromDB,
                               const QByteArray &ownerSecret) const;

    QSet<unsigned int> _checkUserRequestHash;
    QHash<unsigned int, QSharedPointer<V3::UpdateContactData>> _waitResponce ;

};
}
}
#endif // APIV2_H
