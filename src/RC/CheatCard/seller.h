//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SELLER_H
#define SELLER_H
#include "basenode.h"

namespace RC {

namespace API {
class UserHeader;
}

class CHEATCARD_CORE_EXPORT Seller: public BaseNode
{
    Q_OBJECT
public:
    Seller(QH::ISqlDBCache *db);
    bool incrementPurchase (const QSharedPointer<API::UserHeader> &userHeaderData,
                            unsigned int cardId, int purchasesCount = 1,
                            const QString& domain = "",
                            int port = DEFAULT_CHEAT_CARD_PORT_SSL);

    bool sentDataToServerReceive (const QSharedPointer<API::UserHeader> &userHeaderData,
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

    bool setPurchase(const RC::API::UserHeader &userHeaderData,
                     unsigned int cardId, int purchasesCount,
                     const QString& domain = "",
                      int port = DEFAULT_CHEAT_CARD_PORT_SSL);
protected:
    void nodeConnected(QH::AbstractNodeInfo *node) override;
    void nodeConfirmend(QH::AbstractNodeInfo *node) override;
    bool incrementPurchases(const QSharedPointer<API::UsersCards> &usersCardsData,
                            int purchasesCount);

private:
    QSharedPointer<API::UsersCards> prepareData(const RC::API::UserHeader &userHeaderData,
                                                unsigned int cardId);
    bool sendDataPrivate(const QString &domain, int port);

    QHash<long long, QSharedPointer<API::Session>> _lastRequested;

};
}
#endif // SELLER_H
