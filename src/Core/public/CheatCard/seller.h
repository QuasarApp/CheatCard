//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SELLER_H
#define SELLER_H
#include "basenode.h"

namespace RC {

class UserHeader;


class CHEATCARD_CORE_EXPORT Seller: public BaseNode
{
    Q_OBJECT
public:
    Seller(const QSharedPointer<Interfaces::iDB> &db);
    bool incrementPurchase (const QSharedPointer<UserHeader> &userHeaderData,
                           unsigned int cardId, int purchasesCount = 1,
                           const QString& domain = "",
                           int port = DEFAULT_CHEAT_CARD_PORT_SSL);

    bool sentDataToServerReceive (const QSharedPointer<UserHeader> &userHeaderData,
                                 unsigned int cardId, int receiveCount = 1,
                                 const QString& domain = "",
                                 int port = DEFAULT_CHEAT_CARD_PORT_SSL);

    bool requestAllDataFromUser();
    bool cardUpdated(unsigned int cardId, unsigned int version,
                     const QString& domain = "",
                     int port = DEFAULT_CHEAT_CARD_PORT_SSL);

    bool deleteCard(unsigned int cardId,
                    const QString& domain = "",
                    int port = DEFAULT_CHEAT_CARD_PORT_SSL);

    NodeType nodeType() const override;

    bool setPurchase(const UserHeader &userHeaderData,
                     unsigned int cardId, int purchasesCount,
                     const QString& domain = "",
                     int port = DEFAULT_CHEAT_CARD_PORT_SSL);
protected:
    void nodeConnected(QH::AbstractNodeInfo *node) override;
    void nodeConfirmend(QH::AbstractNodeInfo *node) override;
    bool incrementPurchases(const QSharedPointer<Interfaces::iUsersCards> &usersCardsData,
                            int purchasesCount);

private:
    using sendPrivateCB = std::function<bool(const QSharedPointer<Interfaces::iAPI>&,
                                             QH::AbstractNodeInfo *dist)>;

    QSharedPointer<Interfaces::iSession>
    prepareSession(const UserHeader &userHeaderData,
                   unsigned int cardId) const;
    QSharedPointer<Interfaces::iUsersCards>
    prepareData(const UserHeader &userHeaderData,
                unsigned int cardId);
    void updateUsersData(const UserHeader &userHeaderData);

    bool sendDataPrivate(const QString &domain, int port,
                         sendPrivateCB action);

    //    QHash<long long, QSharedPointer<Interfaces::iSession>> _lastRequested;

};
}
#endif // SELLER_H
