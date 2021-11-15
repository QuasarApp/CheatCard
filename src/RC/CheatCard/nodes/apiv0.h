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

class CardStatus;
class Card;
class UsersCards;
class CardDataRequest;
class CardStatusRequest;
class Session;
class User;
class BaseNode;

class CHEATCARD_CORE_EXPORT ApiV0: public QH::iParser
{
    Q_OBJECT
public:
    ApiV0(BaseNode* node);

    BaseNode *node() const;

    int version() const override;

    QH::ParserResult parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                  const QH::Header &pkgHeader,
                                  const QH::AbstractNodeInfo *sender) override;

protected:
    virtual bool processCardStatusRequest(const QSharedPointer<CardStatusRequest> &message,
                           const QH::AbstractNodeInfo *sender, const QH::Header&);

    virtual bool processSession(const QSharedPointer<Session> &message,
                           const QH::AbstractNodeInfo *sender, const QH::Header&);
    virtual bool processCardStatus(const QSharedPointer<QH::PKG::DataPack<UsersCards>> &cardStatuses,
                           const QH::AbstractNodeInfo *sender, const QH::Header&);
    virtual bool applayPurchases(const QSharedPointer<UsersCards> &dbCard,
                         const QH::AbstractNodeInfo *sender);
    virtual bool processCardRequest(const QSharedPointer<CardDataRequest> &cardStatus,
                            const QH::AbstractNodeInfo *sender, const QH::Header&);
    virtual bool processCardData(const QSharedPointer<QH::PKG::DataPack<Card> > &cardrequest,
                         const QH::AbstractNodeInfo *sender, const QH::Header &);

    QH::ISqlDBCache* db() const;

private:

    BaseNode* _node = nullptr;

};
}
#endif // APIV0_H
