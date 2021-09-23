//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#
#ifndef NODE_H
#define NODE_H
#define DB QH::ISqlDBCache

#include <QObject>
#include "CheatCard_global.h"
#include <isqldbcache.h>

namespace RC {

class UsersCards;

enum class NodeStatus {
    UndefinedStatus,
    InProgress,
    Finished
};

enum class Error {
    Successful,
    ConnectionLost,
    TimeOut,
    WrongPackage,
};

class CheatCard_EXPORT Node:  public QObject
{
    Q_OBJECT
public:
    Node(DB *db);

    Error lastError() const;
    void setLastError(Error newLastError);

    NodeStatus lastStatus() const;
    void setLastStatus(NodeStatus newLastStatus);

signals:
    void sigSessionWasFinshed(Error err);
    void sigSessionWasBegin();

protected:
    int getPurchasesCount(unsigned int userId,
                                             unsigned int cardId);
    void beginWork();
    void endWork(Error status);

    QSharedPointer<UsersCards>
    getUserCardData(unsigned int userId, unsigned int cardId);

    DB * _db = nullptr;

private:
    Error _lastError = Error::Successful;
    NodeStatus _lastStatus = NodeStatus::UndefinedStatus;

};
}
#endif // NODE_H
