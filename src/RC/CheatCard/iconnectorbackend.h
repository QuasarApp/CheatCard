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



namespace RC {

class Card;
class ITargetNode;
class User;
class UsersCards;
class CardStatus;
class Session;

class CheatCard_EXPORT IConnectorBackEnd : public QObject
{
    Q_OBJECT
public:

    enum Mode {
        Client,
        Saller,
        ProxyServer,
    };

//    enum Commands {
//        Undefined = 0,
//        UserId = 1,
//        StatusRequest,
//        StatusResponce,
//        CardData,

//        CardDataRequest,

//        Successful
//    };

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
    void setActiveCard(QSharedPointer<Card> newActiveCard, int pusrchasesCount);

    QSharedPointer<User> activeUser() const;
    void setActiveUser(QSharedPointer<User> newActiveUser);

    IConnectorBackEnd::Mode mode() const;
    void setMode(Mode newMode);

    Error lastStatus() const;



    void sigSessionWasFinshed(Error err);
    void sigSessionWasBegin();

protected:

    virtual bool listen(Mode mode) = 0;

    virtual bool close() = 0;

    void reset();

//    void connectionReceived(ITargetNode *obj);
//    void connectionLost(unsigned int nodeID);


    int getPurchasesCount(unsigned int userId,
                          unsigned int cardId);

    QSharedPointer<UsersCards> getUserCardData(unsigned int userId,
                                               unsigned int cardId);



    void beginWork();
    void endWork(Error status);

    Mode _mode = Client;
    int _workID = 0;
    QSharedPointer<Card> _activeCard;
    int _purchasesCount = 0;
    QSharedPointer<User> _activeUser;
    QByteArray _lastSessionId;

    QHash<unsigned long long, unsigned int> _lastUpdates;


    CardStatus* _lastReceivedCardStatus = nullptr;

};

}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(RC::IConnectorBackEnd::Error)
#endif

#endif // ICONNECTORBACKEND_H
