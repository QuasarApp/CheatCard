//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef QRCODERECEIVER_H
#define QRCODERECEIVER_H

#include "firstpackagereceiver.h"

namespace RC {

class QrCodeReceiver: public FirstpackageReceiver
{
    Q_OBJECT
public:
    QrCodeReceiver();

    void setInputQrCodeText(const QString& text);
};
}

#endif // QRCODERECEIVER_H
