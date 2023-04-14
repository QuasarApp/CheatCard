#ifndef IMAGERESPONSE_H
#define IMAGERESPONSE_H

#include <QQuickImageResponse>
#include <QImage>

namespace QH {
class AsyncLauncher;
}

namespace RC {

class DataBase;

namespace Interfaces {
class iDB;
}

/**
 * @brief The ImageResponse class This is image promise
 */
class ImageResponse : public QQuickImageResponse {

    Q_OBJECT
    // QQuickImageResponse interface
public:
    ImageResponse(const QString &id, const QSize &requestedSize,
                  const QSharedPointer<Interfaces::iDB> &db, QThread *thread);
    ~ImageResponse();
    QQuickTextureFactory *textureFactory() const override;
    QString errorString() const override;

public slots:
    void cancel() override;

private:
    QImage prepareImage(const QString &id, const QSize &requestedSize);

    void getDefaultImage(const QString &type, QImage &result) const;
    QH::AsyncLauncher *_launcher = nullptr;
    QSharedPointer<Interfaces::iDB> _db;
    QImage _image;
    QString error;

};
}

#endif // IMAGERESPONSE_H
