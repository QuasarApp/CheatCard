//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "CheatCard_global.h"
#include "itargetnode.h"
#include "node.h"

namespace RC {

class CheatCard_EXPORT Client: public Node
{
    Q_OBJECT
public:
    Client(QH::ISqlDBCache *db);

    virtual void conectToHost() = 0;

signals:
    void sigPurchaseWasSuccessful(QSharedPointer<UsersCards> data);
    void sigCardReceived(QSharedPointer<Card> err);

protected slots:
    void handleReceiveMessage(QByteArray message);
    void handleConnectionClosed(ITargetNode *id);

private:
    bool processCardStatus(const QByteArray &message);

    bool processStatusRequest(const QByteArray &message);

    bool processUserRequest(const QByteArray &message);

    bool processCardRequest(const QByteArray &message);
    bool processCardData(const QByteArray &message);
    bool processSuccessful();

    bool sendCardStatus(const QSharedPointer<UsersCards>& usersCardsData);
    bool sendStatusRequest(const QSharedPointer<Session> &usersCardsData);

    bool incrementPurchases(const QSharedPointer<UsersCards>& usersCardsData);
    bool applayPurchases(QSharedPointer<RC::Card> dbCard,
                         unsigned int purchases);

    QSharedPointer<ITargetNode> _currentTarget;

};
}
#endif // CLIENT_H
