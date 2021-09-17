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
#include "CheatCard_global.h"
#include <QHash>
#include <isqldbcache.h>

#define DB QH::ISqlDBCache

#ifdef QT_DEBUG
#define RC_WAIT_TIME 50000
#else
#define RC_WAIT_TIME 5000
#endif
namespace RC {

class Card;
class ITargetNode;
class User;
class UsersCards;
class CardStatus;

class CheatCard_EXPORT IConnectorBackEnd : public QObject
{
    Q_OBJECT
public:

    enum Mode {
        Client,
        Saller,
    };

    enum Commands {
        Undefined = 0,
        UserId = 1,
        CardData,

        CardDataRequest,

        PurchasesCount,

        Successful
    };

    enum Error {
        UndefinedStatus,
        InProgress,
        FinishedSuccessful,
        ConnectionLost,
        TimeOut,
        WrongPackage,
    };

    IConnectorBackEnd(DB *db);
    ~IConnectorBackEnd();

    bool start(Mode mode);
    bool stop();

    QSharedPointer<Card> activeCard() const;
    void setActiveCard(QSharedPointer<Card> newActiveCard);

    QSharedPointer<User> activeUser() const;
    void setActiveUser(QSharedPointer<User> newActiveUser);

    IConnectorBackEnd::Mode mode() const;
    void setMode(Mode newMode);

    Error lastStatus() const;

signals:
    void sigPurchaseWasSuccessful(QSharedPointer<UsersCards> data);
    void sigCardReceived(QSharedPointer<Card> err);

    void sigSessionWasFinshed(Error err);
    void sigSessionWasBegin();

protected:

    virtual bool listen(Mode mode) = 0;

    virtual bool close() = 0;

    void reset();

    void connectionReceived(ITargetNode *obj);
    void connectionLost(unsigned int nodeID);


    int getPurchasesCount(unsigned int userId,
                          unsigned int cardId);

protected slots:
    void handleReceiveMessage(QByteArray message);
    void handleConnectionClosed(ITargetNode *id);

private:
    bool processCardStatus(const QByteArray &message);

    bool processUserRequest(const QByteArray &message);

    bool processCardRequest(const QByteArray &message);
    bool processCardData(const QByteArray &message);
    bool processSuccessful();

    bool sendCardStatus(const QSharedPointer<UsersCards>& usersCardsData);

    bool incrementPurchases(const QSharedPointer<UsersCards>& usersCardsData);
    bool applayPurchases(QSharedPointer<RC::Card> dbCard,
                         unsigned int purchases);

    void beginWork();
    void endWork(Error status);

    Error _lastStatus = UndefinedStatus;
    Mode _mode = Client;
    int _workID = 0;
    QSharedPointer<ITargetNode> _currentTarget;
    QSharedPointer<Card> _activeCard;
    QSharedPointer<User> _activeUser;

    QHash<unsigned long long, unsigned int> _lastUpdates;

    DB * _db = nullptr;

    CardStatus* _lastReceivedCardStatus = nullptr;

};

}
#endif // ICONNECTORBACKEND_H
