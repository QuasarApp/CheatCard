#include "imageresponse.h"

#include <asynclauncher.h>
#include <thread>
#include <chrono>
#include <QSharedPointer>

#include <rci/core/idb.h>

namespace RC {


ImageResponse::ImageResponse(const QString &id, const QSize &requestedSize,
                             const QSharedPointer<Interfaces::iDB> &db, QThread *thread):
    QQuickImageResponse() {

    _db = db;
    auto task = [id, requestedSize, this]() {
        _image = prepareImage(id, requestedSize);
        emit finished();
        return true;
    };

    _launcher = new QH::AsyncLauncher(thread);
    _launcher->run(task, false);
}

ImageResponse::~ImageResponse() {
    // may be crashes
    delete _launcher;
}

QQuickTextureFactory *ImageResponse::textureFactory() const {
    return QQuickTextureFactory::textureFactoryForImage(_image);
}

QString ImageResponse::errorString() const {
    return QQuickImageResponse::errorString();
}

void ImageResponse::cancel() {
    QQuickImageResponse::cancel();
}

QImage ImageResponse::prepareImage(const QString &id, const QSize &size) {
    auto request = id.split(':', Qt::SkipEmptyParts);

    QImage result;
    auto type = request.value(0);

    if (type == "file") {
        result.load(request.value(1));

        if (size.isValid()) {
            return result.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }

        return result;
    }

    if (_db) {
        QByteArray id = QByteArray::fromBase64(request.value(1).toLatin1());
        auto dbObj = _db->getCardField(id, type);

        if (dbObj.isNull()) {
            getDefaultImage(type, result);
            return result;
        }

        auto array = dbObj.toByteArray();

        if (array.size()) {
            result.loadFromData(array, "PNG");

            if (size.isValid()) {
                return result.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            }
            return result;
        }

        getDefaultImage(type, result);
        return result;
    }

    return result;
};

void ImageResponse::getDefaultImage(const QString &type, QImage& result) const {

    if (type == "logo") {
        result = QImage(":/images/private/resources/CoffeLogo.png");
    } else if (type == "seal") {
        result = QImage(":/images/private/resources/coffeSign.png");
    } else {
        result = QImage();
        result.fill(QColor::fromRgba(0x00000000));
    }
}
}
