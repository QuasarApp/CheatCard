//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SERVER_H
#define SERVER_H

#include "abstractnode.h"
#include "basenode.h"
#include "CheatCard/clearolddata.h"

#include <isqldbcache.h>

namespace RC {

class CHEATCARD_CORE_EXPORT Server: public BaseNode
{
    Q_OBJECT
public:
    Server(QH::ISqlDBCache *db);
    bool cardValidation(const QSharedPointer<Card> &card,
                        const QByteArray &ownerSecret) const override;
    bool sealValidation(const QSharedPointer<UsersCards> &userCardData,
                        const QSharedPointer<Card> &cardFromDb,
                        const QByteArray &ownerSecret) const override;

    // AbstractNode interface
protected:
    void nodeConnected(QH::AbstractNodeInfo *node) override;
    void nodeDisconnected(QH::AbstractNodeInfo *node) override;
    void nodeErrorOccured(QH::AbstractNodeInfo *nodeInfo,
                          QAbstractSocket::SocketError errorCode,
                          QString errorString) override;

    QH::ParserResult parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                  const QH::Header &pkgHeader,
                                  const QH::AbstractNodeInfo *sender) override;

    friend class ClearOldData;
};
}
#endif // SERVER_H
