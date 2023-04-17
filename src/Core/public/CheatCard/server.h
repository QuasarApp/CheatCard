//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SERVER_H
#define SERVER_H

#include "basenode.h"

#include <isqldb.h>

namespace RC {

class CHEATCARD_CORE_EXPORT Server: public BaseNode
{
    Q_OBJECT
public:
    Server(const QSharedPointer<Interfaces::iDB> &db,
           const QVector<unsigned short> &apiVesions);
    NodeType nodeType() const override;

    // AbstractNode interface
    int getMaxCountConnections() const;

    // if user remove own user data but not card
    bool restoreCardsWithotRootSeals();

protected:
    void nodeConfirmend(QH::AbstractNodeInfo *node) override;
    void nodeConnected(QH::AbstractNodeInfo *node) override;
    void nodeDisconnected(QH::AbstractNodeInfo *node) override;
    void nodeErrorOccured(QH::AbstractNodeInfo *nodeInfo,
                          QAbstractSocket::SocketError errorCode,
                          QString errorString) override;


    friend class ClearOldData;

private:
    int maxCountConnections = 0;
};
}
#endif // SERVER_H
