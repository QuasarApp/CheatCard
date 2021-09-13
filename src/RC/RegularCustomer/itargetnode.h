//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef ITARGETNODE_H
#define ITARGETNODE_H

#include <QByteArray>
#include <QObject>

namespace RC {

class ITargetNode: public QObject
{
    Q_OBJECT
public:
    ITargetNode();
    virtual ~ITargetNode();
    virtual bool sendMessage(const QByteArray& array) = 0;
    virtual void close() = 0;
signals:
    void sigMessageReceived(QByteArray message);

};

}
#endif // ITARGETNODE_H
