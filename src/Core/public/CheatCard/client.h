//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef CLIENT_H
#define CLIENT_H

#include "basenode.h"

namespace RC {

/**
 * @brief The Client class contains base functions of clents nodes like a seller or visitor and server address
 */
class CHEATCARD_CORE_EXPORT Client: public BaseNode
{
    Q_OBJECT
public:
    Client(const QSharedPointer<Interfaces::iDB> &db,
           const QVector<unsigned short> &apiVesions);
    ~Client() override;
    bool connectToServer(QString host = {}, int port = 0);
    void disconectFromServer();
    bool isConncted() const;
    bool subscribeToUser(const QByteArray &user);

    bool cardWasUpdated(const QByteArray& cardId);
    bool requestCard(const QByteArray& cardId);

    bool deleteCard(const QByteArray& cardId);

    NodeType nodeType() const override;

    bool incrementPurchase(const QByteArray &userKey,
                           const QByteArray &cardId,
                           int purchasesCount = 1,
                           const std::function<void (int)> &cb = {});


    bool incrementReceived(const QByteArray &userKey,
                           const QByteArray &cardId,
                           int received = 1,
                           const std::function<void (int)> &cb = {});

    void setCurrntUserKey(const QByteArray &newCurrntUserKey);

    bool updateContactData(const QSharedPointer<Interfaces::iContacts> &contact,
                           const QByteArray& secreet,
                           bool removeRequest,
                           const std::function<void (int)> &cb = {});

    bool grantAccess(const QSharedPointer<Interfaces::iUser> &parent,
                     const QByteArray& child, const QString &name = "",
                     const std::function<void (int)> &cb = {});

    bool dropAccess(const QSharedPointer<Interfaces::iUser> &parent,
                    const QByteArray& child,
                    const std::function<void (int)> &cb = {});

    const QByteArray& currntUserKey() const;

    QSharedPointer<RC::Interfaces::iCard> makeCard();


signals:
    void sigAvailableNetworkChanged(bool);

    void sigPurchaseWasSuccessful(QSharedPointer<RC::Interfaces::iUsersCards> data, bool alert);
    void sigCardReceived(QSharedPointer<RC::Interfaces::iCard> card);
    void sigSyncReceived();
    void sigUserDataRemoved(const QByteArray& cardId, const QByteArray& userId);

protected:
    QSharedPointer<Interfaces::iAPI> api() const;

    QH::AbstractNodeInfo *server() const;

    void nodeConnected(QH::AbstractNodeInfo *node) override;
    void nodeConfirmend(QH::AbstractNodeInfo *node) override;

    void nodeDisconnected(QH::AbstractNodeInfo *node) override;

    void nodeErrorOccured(QH::AbstractNodeInfo *nodeInfo,
                          QAbstractSocket::SocketError errorCode,
                          QString errorString) override;

private:
    void setFNetAvailable(bool newFNetAvailable);

    QByteArray _currntUserKey;
    QSharedPointer<Interfaces::iAPI> _api;
    QH::AbstractNodeInfo *_server = nullptr;
    bool _fNetAvailable = false;

    QTimer* _reconnetTimer = nullptr;

};


}
#endif // CLIENT_H
