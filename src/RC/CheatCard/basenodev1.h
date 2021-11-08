#ifndef BASENODEV1_H
#define BASENODEV1_H

#include "basenode.h"


namespace RC {

class BaseNodeV1: public BaseNode
{
    Q_OBJECT
public:
    BaseNodeV1(QH::ISqlDBCache *db);

    // AbstractNode interface
protected:
    QH::AbstractNodeInfo *createNodeInfo(QAbstractSocket *socket,
                                         const QH::HostAddress *clientAddress) const override;

    // BaseNode interface
protected:
    bool processCardStatusRequest(const QSharedPointer<CardStatusRequest> &message,
                                  const QH::AbstractNodeInfo *sender,
                                  const QH::Header &) override;

    bool processSession(const QSharedPointer<Session> &message,
                        const QH::AbstractNodeInfo *sender,
                        const QH::Header &) override;

    bool processCardStatus(const QSharedPointer<QH::PKG::DataPack<UsersCards> > &cardStatuses,
                           const QH::AbstractNodeInfo *sender,
                           const QH::Header &) override;

    bool applayPurchases(const QSharedPointer<UsersCards> &dbCard,
                         const QH::AbstractNodeInfo *sender) override;

    bool processCardRequest(const QSharedPointer<CardDataRequest> &cardStatus,
                            const QH::AbstractNodeInfo *sender,
                            const QH::Header &) override;

    bool processCardData(const QSharedPointer<QH::PKG::DataPack<Card> > &cardrequest,
                         const QH::AbstractNodeInfo *sender,
                         const QH::Header &) override;
};

}

#endif // BASENODEV1_H
