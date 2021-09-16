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
#include <RegularCustomer/database.h>
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
        unsigned int id = request.value(1).toUInt();
        QH::PKG::GetSingleValue request(QH::DbAddress("cards", id), type);
        auto dbObj = _db->db()->getObject(request);

        if (!dbObj || dbObj->value().isNull()) {
            getDefaultImage(type, result);
            return result;
        }

        auto array = dbObj->value().toByteArray();

        if (array.size()) {
            result.loadFromData(array, "PNG");

            if (requestedSize.isValid()) {
                return result.scaled(requestedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            }
            return result;
        }

        getDefaultImage(type, result);
        return result;
    }

    return result;

}

void ImageProvider::getDefaultImage(const QString &type, QPixmap& result) {

    if (type == "logo") {
        result = QPixmap(":/images/private/resources/CoffeLogo.png");
    } else if (type == "seal") {
        result = QPixmap(":/images/private/resources/coffeSign.png");
    } else {
        result = QPixmap(1,1);
        result.fill(QColor::fromRgba(0x00000000));
    }
}
}
