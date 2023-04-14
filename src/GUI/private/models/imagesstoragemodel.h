//
// Copyright (C) 2022-2023 QuasarApp.
// Distributed under the GPLv3 software license, see the accompanying
// Everyone is permitted to copy and distribute verbatim copies
// of this license document, but changing it is not allowed.
//

#ifndef IMAGESSTORAGEMODEL_H
#define IMAGESSTORAGEMODEL_H

#include "basemodel.h"

#include <QStringList>

namespace RC {

/**
 * @brief The ImagesModel class contails all images from resoures
 */
class ImagesStorageModel: public BaseModel
{
public:
    ImagesStorageModel();
    virtual ~ImagesStorageModel();

    const QString& getImageByHash(const QByteArray& anyData);
    const QString& getImageByHash(int anyNumber);

    const QStringList& getImages();

    virtual QStringList initImages() const = 0;

private:
    QStringList _data;
};

}
#endif // IMAGESSTORAGEMODEL_H
