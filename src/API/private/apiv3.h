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
class SyncIncremental;
class UsersCards;
class CardDataRequest;
class Card;
class AuthRequest;
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


    bool sendContacts(const QSharedPointer<Interfaces::iContacts> &conntact,
                      const QByteArray& secreet,
                      bool removeRequest,
                      QH::AbstractNodeInfo *dist,
                      const std::function<void(int err)>& = {}) override;

    bool deleteCard(const QByteArray &cardId,
                    const QByteArray &curentUserKey,
                    QH::AbstractNodeInfo *dist,
                    const std::function<void(int err)>& = {}) override;

    bool requestCard(const QByteArray &cardId,
                     QH::AbstractNodeInfo *dist,
                     const std::function<void(int err)>& = {}) override;

    bool changeUsersData(const QByteArray &sellerUserKey,
                         const QByteArray &cardId,
                         const QByteArray &userId,
                         unsigned int purchasesCount,
                         unsigned int receivedCount,
                         QH::AbstractNodeInfo *dist,
                         const std::function<void(int err)>& = {}) override;

    void sendOldVersionPackage(QH::AbstractNodeInfo *dist) override;


protected:
    bool processAuthRequest(const QSharedPointer<V3::AuthRequest> &message,
                                 QH::AbstractNodeInfo *sender, const QH::Header&) ;


    bool processSync(const QSharedPointer<V3::Sync> &message,
                     const QH::AbstractNodeInfo *sender, const QH::Header&) ;

    bool processSyncIncremental(const QSharedPointer<V3::SyncIncremental> &message,
                                const QH::AbstractNodeInfo *sender, const QH::Header&) ;

    bool processChanges(const QSharedPointer<V3::ChangeUsersCards> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&) ;

    bool processContacts(const QSharedPointer<V3::UpdateContactData> &message,
                         const QH::AbstractNodeInfo *sender, const QH::Header&);

    bool processCardStatus(const QSharedPointer<QH::PKG::DataPack<V3::UsersCards>> &cardStatuses,
                           const QH::AbstractNodeInfo *sender, const QH::Header&pkg);

    bool processCardRequest(const QSharedPointer<V3::CardDataRequest> &cardStatus,
                            const QH::AbstractNodeInfo *sender, const QH::Header&);

    bool processCardData(const QSharedPointer<QH::PKG::DataPack<V3::Card> > &cardrequest,
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

    bool sendFullSync(const QByteArray &userId, const QH::AbstractNodeInfo *sender, const QH::Header &hdr);

    /**
     * @brief brodcastUserChanged This method sent message to all subcribed nodes that connect write now.
     * @param objId this is id of the user or card that data is changed.
     * @param data This is changes of the data.
     * @param req This is req header of the trigger node.
     * @param alredySentNodes This is set of the nodes that alredy get this changes.
     */
    bool brodcastUserChanged(const QSet<QByteArray>& objId,
                             const QH::PKG::AbstractData *data,
                             const QH::Header *req,
                             QSet<const QH::AbstractNodeInfo *> &alredySentNodes);

    /**
     * @brief subscribePrivete This method subscribe user to changes of the object.
     * @param objectId This is object id.
     * @param subscriber this is subscriber that need to subscribe to object
     */
    void subscribePrivete(const QByteArray &objectIds,
                          const QByteArray &listner,
                          const QH::AbstractNodeInfo *subscriber);

    void subscribePrivete(const QH::PKG::DataPack<API::V3::UsersCards> &objectIds,
                          const QByteArray &listner,
                          const QH::AbstractNodeInfo *subscriber);

    void refreshSubscribes(const QByteArray &objectId,
                           const QH::AbstractNodeInfo *subscriber);


    bool processCardUpdatePrivate(const QByteArray& card, unsigned int version,
                                  const QH::AbstractNodeInfo *sender, const QH::Header &) ;

    unsigned int sendAndRegisterCallBack(const QH::PKG::AbstractData *resp,
                                         const QH::AbstractNodeInfo *address,
                                         const std::function<void(int err)>& cb);

    bool triggerCallBack(unsigned int hash, unsigned int err);

    void collectDataOfuser(const QByteArray &userKey,
                           QH::PKG::DataPack<API::V3::UsersCards> &responce);


    bool applayPurchases(const QSharedPointer<API::V3::UsersCards> &dbCard,
                         const QH::AbstractNodeInfo *sender, bool alert = true);

    bool processCardStatusBase(const QSharedPointer<V3::UsersCards> &cardStatus,
                               const QByteArray &userSecreet,
                               const QH::AbstractNodeInfo *sender,
                               const QH::Header &pkg,
                               QByteArray *neededCardId = nullptr,
                               QSharedPointer<Interfaces::iCard> *changedCard = nullptr);

    bool cardValidation(const QSharedPointer<RC::Interfaces::iCard> &cardFromDB,
                        const QByteArray &ownerSecret) const;

    QH::AbstractNodeInfo *getUser(const QByteArray& userId);
    void auth(const QByteArray& userId, QH::AbstractNodeInfo *userNode);

    void restoreSubscribes();

    struct RequestsData {
        int time = 0;
        std::function<void(int err)> _cb;
    };

    QSet<unsigned int> _checkUserRequestHash;
    QHash<unsigned int, RequestsData> _waitResponce;

    QMutex _subscribesMutex;

    QHash<QByteArray/* chaned object id */,
          QHash<QByteArray/* listner id*/,
                QSet<const QH::AbstractNodeInfo*> /*listner sockets*/>> _subscribes;

    QMutex _usersMutex;
    QHash<QByteArray, QH::AbstractNodeInfo*> _users;

};
}
}
#endif // APIV2_H
