//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef ICONNECTORBACKEND_H
#define ICONNECTORBACKEND_H

#include <QByteArray>

namespace RC {

class IConnectorBackEnd
{
public:
    IConnectorBackEnd();

protected:

    virtual void receiveMessage(const QByteArray& message) = 0;
    virtual bool sendMessage(const QByteArray& message) = 0;
};

}
#endif // ICONNECTORBACKEND_H
