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

class UserHeader;

class CHEATCARD_CORE_EXPORT Seller: public BaseNode
{
    Q_OBJECT
public:
    Seller(QH::ISqlDBCache *db);
    bool incrementPurchase (const QSharedPointer<UserHeader> &userHeaderData,
                            unsigned int cardId, int purchasesCount = 1,
                            const QString& domain = "",
                            int port = DEFAULT_CHEAT_CARD_PORT_SSL);

    bool sentDataToServerPurchase (const QSharedPointer<UserHeader> &userHeaderData,
                            unsigned int cardId, const QString& domain = "",
                            int port = DEFAULT_CHEAT_CARD_PORT_SSL);

    bool requestAllDataFromUser();

    bool cardValidation(const QSharedPointer<Card> &card,
                        const QByteArray &ownerSecret) const override;
    void getSignData(QByteArray &data) const override;

protected:
    void nodeConnected(QH::AbstractNodeInfo *node) override;
    void nodeConfirmend(QH::AbstractNodeInfo *node) override;
    bool incrementPurchases(const QSharedPointer<UsersCards> &usersCardsData,
                            int purchasesCount);

private:
    QSharedPointer<UsersCards> prepareData(const QSharedPointer<UserHeader> &userHeaderData,
                         unsigned int cardId);

    QString randomUserName() const;

    QHash<long long, QSharedPointer<Session>> _lastRequested;

};
}
#endif // SELLER_H
