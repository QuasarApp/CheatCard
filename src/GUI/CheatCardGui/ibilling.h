//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include <QObject>


#ifndef IBILLING_H
#define IBILLING_H

namespace RC {

struct Purchase {
    QString id;
    QString token;
};

class IBilling: public QObject
{
    Q_OBJECT
public:
    IBilling();
    virtual void init() = 0;

public slots:
    virtual void becomeSeller() = 0;

signals:
    void sigPurchaseReceived(RC::Purchase);

};

}

Q_DECLARE_METATYPE(RC::Purchase)
#endif // IBILLING_H
