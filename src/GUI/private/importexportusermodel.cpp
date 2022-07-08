//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "importexportusermodel.h"
#include "iplatformtools.h"

#include <QPixmap>
#include <QUrl>
#include <qmlnotifyservice.h>
#include <QDebug>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif


namespace RC {
ImportExportUserModel::ImportExportUserModel() {
    connect(&_decoder, &SBarcodeDecoder::capturedChanged,
            this, &ImportExportUserModel::handleDecodeFinished);
}

bool ImportExportUserModel::processQrCode(QString path) {
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

    if (!_decoder.process(tmpImage.toImage())) {
        service->setNotify(tr("Oh shit"),
                           tr("It looks like the file you selected does not contain a recovery code"),
                           "",
                           QmlNotificationService::NotificationData::Error);
        return false;
    }

    return true;
}

void ImportExportUserModel::handleDecodeFinished(const QString &data) {
    emit decodeFinished(data);
}
}


