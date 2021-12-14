//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef INETIDICATORMODEL_H
#define INETIDICATORMODEL_H

#include <QObject>

class QObject;

namespace RC {

class InetIdicatorModel: public QObject
{
    Q_PROPERTY(bool enableNetwork READ getEnableNetwork WRITE setEnableNetwork NOTIFY EnableNetworkChanged)

public:
    InetIdicatorModel();

    bool getEnableNetwork() const;
    void setEnableNetwork(bool enable);

signals:
    void EnableNetworkChanged();

private:
    bool enableNetwork = false;

};
}

#endif // INETIDICATORMODEL_H
