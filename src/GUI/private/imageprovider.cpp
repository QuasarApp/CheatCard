//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "imageprovider.h"
#include <QQuickImageResponse>
#include <QtConcurrent>
#include "imageresponse.h"
#include <qaglobalutils.h>

namespace RC {


ImageProvider::ImageProvider(const QSharedPointer<Interfaces::iDB> &db):
    QQuickAsyncImageProvider() {

    debug_assert(db, "The database must be not null");

    _db = db;
    _imageLoaderThread = new QThread();
    _imageLoaderThread->setObjectName("imageLoaderThread");
    _imageLoaderThread->start();
}

QQuickImageResponse *ImageProvider::requestImageResponse(const QString &id,
                                                         const QSize &requestedSize) {

    return new class ImageResponse(id, requestedSize, _db, _imageLoaderThread);
};

ImageProvider::~ImageProvider() {

    _imageLoaderThread->quit();
    _imageLoaderThread->wait();

    delete _imageLoaderThread;
};

}
