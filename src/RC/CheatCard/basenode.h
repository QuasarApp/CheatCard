//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef BASENODE_H
#define BASENODE_H
#include "abstractnode.h"
#include "core_global.h"
#include "datapack.h"
#include <isqldbcache.h>

#define TEST_SELLER_TOKEN "testbase64tokenofseller"
#define USERREQUEST_TIMEOUT 10
namespace RC {

class CardStatus;
class Card;
class UsersCards;
class CardDataRequest;
class CardStatusRequest;
class Session;
class User;

class CHEATCARD_CORE_EXPORT BaseNode: public QH::AbstractNode
{
    Q_OBJECT
public:
    BaseNode(QH::ISqlDBCache *db);
    int getFreeItemsCount(unsigned int userId, unsigned int cardId) const;
    int getFreeItemsCount(const QSharedPointer<UsersCards>& inputData) const;
    int getFreeItemsCount(const QSharedPointer<UsersCards>& inputData, unsigned int freeIndex) const;
    int getCountOfReceivedItems(unsigned int userId, unsigned int cardId);

    int getCardFreeIndex(unsigned int cardId) const;
    unsigned int getCardVersion(unsigned int cardId) const;

    static QString libVersion();

    QSharedPointer<UsersCards>
    getUserCardData(unsigned int userId, unsigned int cardId) const;
    QList<QSharedPointer<UsersCards> >
    getAllUserFromCard(unsigned int cardId, bool includeOwner = true) const;

    QList<QSharedPointer<User> >
    getAllUserDataFromCard(unsigned int cardId, bool includeOwner = true) const;

    QSharedPointer<Card> getCard(unsigned int cardId);

signals:
    void sigPurchaseWasSuccessful(QSharedPointer<RC::UsersCards> data);
    void sigCardReceived(QSharedPointer<RC::Card> err);


protected:
    QH::ISqlDBCache *db() const;
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

    QH::ParserResult parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                  const QH::Header &pkgHeader,
                                  const QH::AbstractNodeInfo *sender) override;

    // AbstractNode interface
protected:
    QH::AbstractNodeInfo *createNodeInfo(QAbstractSocket *socket,
                                         const QH::HostAddress *clientAddress) const override;

private:
    QH::ISqlDBCache *_db = nullptr;


};
}
#endif // BASENODE_H
