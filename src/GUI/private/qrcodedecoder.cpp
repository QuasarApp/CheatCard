//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "qrcodedecoder.h"
#include "iplatformtools.h"

#include <QPixmap>
#include <QUrl>
#include <qmlnotifyservice.h>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif

namespace RC {

QRCodeDecoder::QRCodeDecoder()
{
    connect(&_decoder, &SBarcodeDecoder::capturedChanged,
            this, &QRCodeDecoder::handleDecodeFinished);
}

bool QRCodeDecoder::processQrCode(QString path) {
    QUrl url(path);
    auto service = QmlNotificationService::NotificationService::getService();

    auto platformsTools = IPlatformTools::instance();
    if (!platformsTools->getAccessToReadInStorage()){

        service->setNotify(tr("Oh shit"),
                           tr("Permission denied to selected file."),
                           "",
                           QmlNotificationService::NotificationData::Error);
        return false;
    }

#ifdef Q_OS_ANDROID
    QPixmap tmpImage((url.scheme() == "qrc")? ":" + QUrl(path).path(): path);
#else
    QPixmap tmpImage((url.scheme() == "qrc")? ":" + QUrl(path).path(): QUrl(path).path());
#endif

    if (!(_decoder.process(tmpImage.toImage()) && _decoder.captured().size())) {
        service->setNotify(tr("Oh shit"),
                           tr("It looks like the file you selected does not contain a qr code"),
                           "",
                           QmlNotificationService::NotificationData::Error);
        return false;
    }


    return true;
}

void QRCodeDecoder::handleDecodeFinished(const QString &data) {
    emit decodeFinished(data);
}
}
