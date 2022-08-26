//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef QRCODEDECODER_H
#define QRCODEDECODER_H

#include "SBarcodeDecoder.h"
#include <QObject>

namespace RC {

class QRCodeDecoder: public QObject {

    Q_OBJECT
public:
    QRCodeDecoder();
    Q_INVOKABLE bool processQrCode(QString path);

public slots:
    virtual void handleDecodeFinished(const QString &data);

signals:
    void decodeFinished(QString data);

private:
    SBarcodeDecoder _decoder;
};
}
#endif // QRCODEDECODER_H
