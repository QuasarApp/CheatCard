//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef APIV0_H
#define APIV0_H
#include "abstractnode.h"
#include "CheatCard/core_global.h"
#include "datapack.h"
#include <isqldbcache.h>

#define USERREQUEST_TIMEOUT 3


namespace RC {

namespace API {
class CardStatus;
class Card;
class UsersCards;
class CardDataRequest;
class CardStatusRequest;
class Session;
class User;
class UserHeader;
}

class BaseNode;
class IAPIObjectsFactory;

class CHEATCARD_CORE_EXPORT ApiV0: public QH::iParser
{
public:
    ApiV0(BaseNode* node);
    virtual ~ApiV0();

    int version() const override;

    QH::ParserResult parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                  const QH::Header &pkgHeader,
                                  const QH::AbstractNodeInfo *sender) override;

    // using on visitor
    virtual void sendCardStatusRequest(long long userSession, QH::AbstractNodeInfo *dist);

    // using on seller
    virtual void sendSessions(const QHash<long long, QSharedPointer<API::Session> > &sessions, QH::AbstractNodeInfo *dist);



protected:
    BaseNode *node() const;
    IAPIObjectsFactory *objectFactoryInstance();
    virtual IAPIObjectsFactory *initObjectFactory() const;

    bool processCardStatusRequest(const QSharedPointer<API::CardStatusRequest> &message,
                                  const QH::AbstractNodeInfo *sender, const QH::Header&);

    bool processSession(const QSharedPointer<API::Session> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&);
    bool processCardStatus(const QSharedPointer<QH::PKG::DataPack<API::UsersCards>> &cardStatuses,
                           const QH::AbstractNodeInfo *sender, const QH::Header&);
    bool applayPurchases(const QSharedPointer<API::UsersCards> &dbCard,
                         const QH::AbstractNodeInfo *sender, bool alert = true);
    bool processCardRequest(const QSharedPointer<API::CardDataRequest> &cardStatus,
                            const QH::AbstractNodeInfo *sender, const QH::Header&);
    bool processCardData(const QSharedPointer<QH::PKG::DataPack<API::Card> > &cardrequest,
                         const QH::AbstractNodeInfo *sender, const QH::Header &);

    QH::ISqlDBCache* db() const;
    QSharedPointer<API::UsersCards>
    userHeaderReceived(const QSharedPointer<API::UserHeader> &userHeaderData,
                       unsigned int cardId);

    /**
     * @brief sessionProcessed invoke when session has been processed
     */
    virtual void sessionProcessed(unsigned long long sessionId);
private:

    BaseNode* _node = nullptr;
    IAPIObjectsFactory *_objectFactory = nullptr;

};
}
#endif // APIV0_H
