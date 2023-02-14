//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef CLIENT_H
#define CLIENT_H

#include "basenode.h"
#include "userheader.h"

namespace RC {

/**
 * @brief The Client class contains base functions of clents nodes like a seller or visitor and server address
 */
class CHEATCARD_CORE_EXPORT Client: public BaseNode
{
public:
    Client(const QSharedPointer<Interfaces::iDB> &db);
    bool connectToServer();
    void disconectFromServer();
    bool isConncted() const;
    bool subscribeToUser(const QByteArray &user) const;

    bool updateCard(const QByteArray& cardId, unsigned int version);
    bool deleteCard(const QByteArray& cardId);

    NodeType nodeType() const override;

    bool setPurchase(const UserHeader &userHeaderData,
                     const QByteArray &cardId,
                     int purchasesCount);

    bool incrementPurchase(const QSharedPointer<UserHeader> &userHeaderData,
                           unsigned int cardId,
                           int purchasesCount = 1);

    void setCurrntUser(const QSharedPointer<Interfaces::iUser> &newCurrntUser);

    void setCurrntUserKey(const QByteArray &newCurrntUserKey);

signals:
    void sigAvailableNetworkChanged(bool);

    void sigPurchaseWasSuccessful(QSharedPointer<RC::Interfaces::iUsersCards> data, bool alert);
    void sigCardReceived(QSharedPointer<RC::Interfaces::iCard> card);
    void sigContactsListChanged();
    void sigUserDataRemoved(const QByteArray& cardId, const QByteArray& userId);

protected:
    QSharedPointer<Interfaces::iAPI> api() const;

    QH::AbstractNodeInfo *server() const;

    void nodeConnected(QH::AbstractNodeInfo *node) override;
    void nodeDisconnected(QH::AbstractNodeInfo *node) override;
    QString getServerHost() const;

private:
    void setFNetAvailable(bool newFNetAvailable);

    QByteArray _currntUserKey;
    QSharedPointer<Interfaces::iAPI> _api;
    QH::AbstractNodeInfo *_server = nullptr;
    bool _fNetAvailable = false;

};

}
#endif // CLIENT_H
