//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef BASENODE_H
#define BASENODE_H
#include "abstractnode.h"

#include <isqldbcache.h>
namespace RC {

class CardStatus;
class Card;
class UsersCards;
class CardDataRequest;
class CardStatusRequest;

class BaseNode: public QH::AbstractNode
{
    Q_OBJECT
public:
    BaseNode(QH::ISqlDBCache *db);

signals:
    void sigPurchaseWasSuccessful(QSharedPointer<UsersCards> data);
    void sigCardReceived(QSharedPointer<Card> err);

protected:
    QH::ISqlDBCache *db() const;
    bool processCardStatusRequest(const QSharedPointer<CardStatusRequest> &message,
                           const QH::AbstractNodeInfo *sender, const QH::Header&);
    bool processCardStatus(const QSharedPointer<UsersCards> &message,
                           const QH::AbstractNodeInfo *sender, const QH::Header&);
    bool applayPurchases(const QSharedPointer<UsersCards> &dbCard,
                         const QH::AbstractNodeInfo *sender);
    bool processCardRequest(const QSharedPointer<CardDataRequest> &cardStatus,
                            const QH::AbstractNodeInfo *sender, const QH::Header&);
    bool processCardData(const QSharedPointer<Card> &cardrequest,
                         const QH::AbstractNodeInfo *sender, const QH::Header &);

    QSharedPointer<UsersCards>
    getUserCardData(unsigned int userId, unsigned int cardId);

    QSharedPointer<Card> getCard(unsigned int cardId);

private:
    QH::ISqlDBCache *_db = nullptr;
};
}
#endif // BASENODE_H
