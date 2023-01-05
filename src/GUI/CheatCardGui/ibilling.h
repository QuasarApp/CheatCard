//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef IBILLING_H
#define IBILLING_H

#include <QObject>
#include "gui_global.h"

namespace RC {

struct Purchase {
    QString id;
    QString token;
};

class CHEATCARD_GUI_EXPORT IBilling: public QObject
{
    Q_OBJECT
public:
    IBilling();
    virtual void init() = 0;

public slots:
    virtual void becomeSeller() = 0;
    virtual bool isSupported() = 0;

signals:
    void sigPurchaseReceived(RC::Purchase);

};

}

Q_DECLARE_METATYPE(RC::Purchase)
#endif // IBILLING_H
