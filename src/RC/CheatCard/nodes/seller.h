//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef SELLER_H
#define SELLER_H
#include "basenodev1.h"

namespace RC {

class UserHeader;

class CHEATCARD_CORE_EXPORT Seller: public BaseNodeV1
{
    Q_OBJECT
public:
    Seller(QH::ISqlDBCache *db);
    bool incrementPurchase (const QSharedPointer<UserHeader> &userHeaderData,
                            unsigned int cardId, int purchasesCount = 1,
                            const QString& domain = DEFAULT_CHEAT_CARD_HOST,
                            int port = DEFAULT_CHEAT_CARD_PORT_SSL);

protected:
    void nodeConnected(QH::AbstractNodeInfo *node) override;
    bool incrementPurchases(const QSharedPointer<UsersCards> &usersCardsData,
                            int purchasesCount);

private:

    QString randomUserName() const;

    QHash<long long, QSharedPointer<Session>> _lastRequested;


};
}
#endif // SELLER_H
