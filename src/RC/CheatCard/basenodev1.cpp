#include "basenodev1.h"

namespace RC {

BaseNodeV1::BaseNodeV1(QH::ISqlDBCache *db): BaseNode(db)
{

}

QH::AbstractNodeInfo *RC::BaseNodeV1::createNodeInfo(QAbstractSocket *socket,
                                                     const QH::HostAddress *clientAddress) const {
    return BaseNode::createNodeInfo(socket, clientAddress);
}

bool BaseNodeV1::processCardStatusRequest(const QSharedPointer<CardStatusRequest> &message,
                                          const QH::AbstractNodeInfo *sender,
                                          const QH::Header &hdr) {
    return BaseNode::processCardStatusRequest(message, sender, hdr);

}

bool BaseNodeV1::processSession(const QSharedPointer<Session> &message,
                                const QH::AbstractNodeInfo *sender,
                                const QH::Header &hdr) {
    return BaseNode::processSession(message, sender, hdr);

}

bool BaseNodeV1::processCardStatus(const QSharedPointer<QH::PKG::DataPack<UsersCards> > &cardStatuses,
                                   const QH::AbstractNodeInfo *sender,
                                   const QH::Header &hdr) {
    return BaseNode::processCardStatus(cardStatuses, sender, hdr);

}

bool BaseNodeV1::applayPurchases(const QSharedPointer<UsersCards> &dbCard,
                                 const QH::AbstractNodeInfo *sender) {
    return BaseNode::applayPurchases(dbCard, sender);

}

bool BaseNodeV1::processCardRequest(const QSharedPointer<CardDataRequest> &cardStatus,
                                    const QH::AbstractNodeInfo *sender,
                                    const QH::Header &hdr) {
    return BaseNode::processCardRequest(cardStatus, sender, hdr);

}

bool BaseNodeV1::processCardData(const QSharedPointer<QH::PKG::DataPack<Card> > &cardrequest,
                                 const QH::AbstractNodeInfo *sender,
                                 const QH::Header &hdr) {
    return BaseNode::processCardData(cardrequest, sender, hdr);

}
}
