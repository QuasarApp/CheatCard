//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include "rci/core/idb.h"
#include <QQuickAsyncImageProvider>
#include <QPixmap>
#include <async.h>

namespace RC {

class DataBase;

class ImageProvider: public QQuickAsyncImageProvider {

public:
    explicit ImageProvider(const QSharedPointer<Interfaces::iDB>& db);
    ~ImageProvider() override;

    QQuickImageResponse *requestImageResponse(const QString &id,
                                              const QSize &requestedSize) override;
private:
    QSharedPointer<Interfaces::iDB> _db;
    QThread *_imageLoaderThread = nullptr;

};

}
#endif // IMAGEPROVIDER_H
