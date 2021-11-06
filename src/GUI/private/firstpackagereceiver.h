//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef FIRSTPACKAGERECEIVER_H
#define FIRSTPACKAGERECEIVER_H

#include <QObject>
namespace RC {

class FirstpackageReceiver: public QObject
{
    Q_OBJECT
public:
    FirstpackageReceiver();

signals:

    void sigDataReceived(QByteArray data);
    void sigDataSendet(QByteArray data);

};
}
#endif // FIRSTPACKAGERECEIVER_H
