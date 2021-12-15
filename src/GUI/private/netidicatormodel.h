//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef NETIDICATORMODEL_H
#define NETIDICATORMODEL_H

#include <QObject>

namespace RC {

class NetIdicatorModel: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enableNetwork READ getEnableNetwork WRITE setEnableNetwork NOTIFY EnableNetworkChanged)

public:
    NetIdicatorModel();

    bool getEnableNetwork() const;
    void setEnableNetwork(bool enable);

signals:
    void EnableNetworkChanged();

private:
    bool enableNetwork = false;

};
}

#endif // NETIDICATORMODEL_H
