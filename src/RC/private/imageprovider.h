//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QPixmap>

namespace RC {

class DataBase;

class ImageProvider: public QQuickImageProvider {
    Q_OBJECT
public:
    explicit ImageProvider(DataBase* db);
    ~ImageProvider() override;

    QPixmap requestPixmap(const QString &id,
                          QSize *size,
                          const QSize &requestedSize) override;

private:

    void getDefaultImage(const QString &type, QPixmap &result);

    DataBase *_db = nullptr;
};

}
#endif // IMAGEPROVIDER_H
