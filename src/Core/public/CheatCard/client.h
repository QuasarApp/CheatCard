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
public:
    Client(const QSharedPointer<Interfaces::iDB> &db);
    bool connectToServer();
    void disconectFromServer();
    bool isConncted() const;

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

    QSharedPointer<Interfaces::iAPI> _api;
    QH::AbstractNodeInfo *_server = nullptr;
    bool _fNetAvailable = false;

};

}
#endif // CLIENT_H
