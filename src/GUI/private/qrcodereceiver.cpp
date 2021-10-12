//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "qrcodereceiver.h"

namespace RC {

QrCodeReceiver::QrCodeReceiver() {

}

void QrCodeReceiver::setInputQrCodeText(const QString &text) {
    emit sigDataReceived(QByteArray::fromHex(text.toLatin1()));
}
}
