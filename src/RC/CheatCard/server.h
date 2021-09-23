//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "CheatCard_global.h"
#include "node.h"

namespace RC {

class ITargetNode;

class CheatCard_EXPORT Server: public Node
{
    Q_OBJECT
public:
    Server(DB *db);
    ~Server();

    virtual bool listen() = 0;
    virtual bool close() = 0;

protected:
    void connectionReceived(ITargetNode *obj);
    void connectionLost(unsigned int nodeID);

};

}
#endif // SERVER_H
