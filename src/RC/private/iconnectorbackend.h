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

    enum Mode {
        Client,
        Saller,
    };

    IConnectorBackEnd();

    bool start(Mode mode);
    bool stop();

protected:

    virtual bool listen(Mode mode) = 0;

    virtual bool close() = 0;

    void receiveMessage(const QByteArray& message);
    bool sendMessage(const QByteArray& message);

    void connectionReceived(const QByteArray& id);
    void connectionLost(const QByteArray& id);
};

}
#endif // ICONNECTORBACKEND_H
