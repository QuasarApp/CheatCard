//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef BASENODE_H
#define BASENODE_H
#include "abstractnode.h"
#include "CheatCard/core_global.h"
#include "rci/core/idb.h"
#include <QSharedPointer>
#include <dbobject.h>
#include <QSslError>
#include <rci/objects/iuser.h>

namespace RC {

namespace Interfaces{
class iAPI;
}

class CHEATCARD_CORE_EXPORT BaseNode: public QH::AbstractNode
{
    Q_OBJECT
public:
    BaseNode(const QSharedPointer<Interfaces::iDB> &dataBase);
    const QSharedPointer<Interfaces::iDB>& db() const;

    static QString libVersion();
signals:
    void sigNetworkError(unsigned int errorCode,
                         QSslError::SslError sslError);


    // AbstractNode interface
protected slots:
    void nodeErrorOccured(QH::AbstractNodeInfo *nodeInfo,
                          QAbstractSocket::SocketError errorCode,
                          QString errorString) override;

    void handleSslErrorOcurred(QH::SslSocket *scket, const QSslError &error) override;

    QString getServerHost() const;
private:

    QSharedPointer<Interfaces::iDB> _db = nullptr;
};
}
#endif // BASENODE_H
