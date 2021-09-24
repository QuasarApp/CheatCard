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
class Session;
class RawData;
class UserHeader;

class CheatCard_EXPORT IConnectorBackEnd : public QObject
{
    Q_OBJECT
public:

    enum Mode {
        Client,
        Saller,
        ProxyServer,
    };

    enum Commands {
        Undefined = 0,
        UserId = 1,
        StatusRequest,
        StatusResponce,
        CardData,

        CardDataRequest,

        Successful
    };

    enum Status {
        UndefinedStatus,
        InProgress,
        FinishedSuccessful,
        ConnectionLost,
        TimeOut,
        WrongPackage,
    };

    IConnectorBackEnd(DB *db);
    ~IConnectorBackEnd();

    bool start();
    bool stop();

    QSharedPointer<Card> activeCard() const;
    void setActiveCard(QSharedPointer<Card> newActiveCard, int pusrchasesCount);

    QSharedPointer<User> activeUser() const;
    void setActiveUser(QSharedPointer<User> newActiveUser);

    Status lastStatus() const;

    const QSharedPointer<ITargetNode> &currentTarget() const;

    unsigned long long lastSessionId() const;
    void setLastSessionId(unsigned long long newLastSessionId);

signals:
    void sigPurchaseWasSuccessful(QSharedPointer<UsersCards> data);
    void sigCardReceived(QSharedPointer<Card> err);

    void sigSessionWasFinshed(Status err);
    void sigSessionWasBegin();

protected:

    virtual bool listen() = 0;
    virtual bool hello() = 0;
    virtual bool close() = 0;

    IConnectorBackEnd::Mode mode() const;
    void setMode(Mode newMode);

    void reset();

    void connectionReceived(ITargetNode *obj);
    void connectionLost(unsigned int nodeID);


    int getPurchasesCount(unsigned int userId,
                          unsigned int cardId);

    QSharedPointer<UsersCards> getUserCardData(unsigned int userId,
                                               unsigned int cardId);

    bool generateSesionData(UserHeader & data) const;
    QByteArray sessionDataToArray(const UserHeader& data);
    bool sessionFromArray(const QByteArray& inputData, UserHeader &outPutData);

    bool sendStatusRequest(const QSharedPointer<Session> &usersCardsData);
    bool sendStatusRequest(unsigned long long sessionId);

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

    template <typename  Type>
    bool sendCardStatus(const QList<QSharedPointer<Type>> &usersCardsData) {
        QByteArray array;
        QDataStream stream(&array, QIODevice::WriteOnly);
        stream << static_cast<int>(usersCardsData.size());
        for (auto data : usersCardsData) {
            stream << *data.data();
        }

        return sendRawDataPackage(IConnectorBackEnd::StatusResponce, array);
    }

    bool incrementPurchases(const QSharedPointer<UsersCards>& usersCardsData);
    bool applayPurchases(QSharedPointer<RC::Card> dbCard,
                         unsigned int purchases);

    bool sendRawDataPackage(Commands command, const QByteArray& data);
    bool extractRawData(const QByteArray& data, RawData& result);

    void beginWork();
    void endWork(Status status);

    Status _lastStatus = UndefinedStatus;
    Mode _mode = Client;
    int _workID = 0;
    QSharedPointer<ITargetNode> _currentTarget;
    QSharedPointer<Card> _activeCard;
    int _purchasesCount = 0;
    QSharedPointer<User> _activeUser;
    unsigned long long _lastSessionId;

    QHash<unsigned long long, unsigned int> _lastUpdates;

    DB * _db = nullptr;

    CardStatus* _lastReceivedCardStatus = nullptr;

};

}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(RC::IConnectorBackEnd::Status)
#endif

#endif // ICONNECTORBACKEND_H
