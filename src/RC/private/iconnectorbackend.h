//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef ICONNECTORBACKEND_H
#define ICONNECTORBACKEND_H

#include <QByteArray>
#include <QSharedPointer>
#include <database.h>

namespace RC {

class Card;
class ITargetNode;
class User;
class UsersCards;

class IConnectorBackEnd : public QObject
{
    Q_OBJECT
public:

    enum Mode {
        Client,
        Saller,
    };

    enum Commands {
        UserId,
        SallerId,
        CardId,
        CardData,

        CardDataRequest,

        PurchasesCount,

        Successful
    };

    IConnectorBackEnd(DB *db);

    bool start(Mode mode);
    bool stop();

    QSharedPointer<Card> activeCard() const;
    void setActiveCard(QSharedPointer<Card> newActiveCard);

    QSharedPointer<User> activeUser() const;
    void setActiveUser(QSharedPointer<User> newActiveUser);

signals:
    void sigUserPurchaseWasSuccessful(QSharedPointer<User>);
    void sigCardPurchaseWasSuccessful(QSharedPointer<Card>);

protected:

    virtual bool listen(Mode mode) = 0;

    virtual bool close() = 0;

    void connectionReceived( ITargetNode *obj);
    void connectionLost(ITargetNode* id);

protected slots:
    void handleReceiveMessage(const QByteArray& message);

private:
    bool workWithCardStatus(const QByteArray &message);

    bool workWithUserRequest(const QByteArray &message);

    bool sendCardStatus(const QSharedPointer<UsersCards>& usersCardsData);

    bool incrementPurchases(const QSharedPointer<UsersCards>& usersCardsData);

    Mode _mode = Client;
    QSharedPointer<ITargetNode> _currentTarget;
    QSharedPointer<Card> _activeCard;
    QSharedPointer<User> _activeUser;

    QHash<unsigned long long, unsigned int> _lastUpdates;

    DB * _db = nullptr;

};

}
#endif // ICONNECTORBACKEND_H
