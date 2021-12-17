//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef NETINDICATORMODEL_H
#define NETINDICATORMODEL_H

#include <QObject>

namespace RC {

class NetIndicatorModel: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enableNetwork READ getEnableNetwork WRITE setEnableNetwork NOTIFY enableNetworkChanged)

public:
    NetIndicatorModel();

    bool getEnableNetwork() const;
    void setEnableNetwork(bool enable);

signals:
    void enableNetworkChanged();

private:
    bool enableNetwork = true;

};

}

#endif // NETINDICATORMODEL_H
