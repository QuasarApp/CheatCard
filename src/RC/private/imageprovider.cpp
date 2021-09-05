//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "imageprovider.h"
#include <QQuickImageResponse>
#include <QtConcurrent>
#include "quasarapp.h"
#include "database.h"
#include "getsinglevalue.h"

namespace RC {

ImageProvider::ImageProvider(DataBase* db):
    QQuickImageProvider(QQuickImageProvider::Pixmap) {

    debug_assert(db, "The database must be not null");

    _db = db;
};
ImageProvider::~ImageProvider() = default;


QPixmap ImageProvider::requestPixmap(const QString &id,
                                     QSize *,
                                     const QSize &requestedSize) {

    auto request = id.split('/');

    QPixmap result(1,1);
    auto type = request.value(0);

    if (_db) {
        int id = request.value(1).toInt();
        QH::PKG::GetSingleValue request(QH::DbAddress("cards", id), type);
        _db->db()->getObject(request);
        result.loadFromData(request.value().toByteArray(), "PNG");

        return result.scaled(requestedSize);
    }

    return result;

}
}
