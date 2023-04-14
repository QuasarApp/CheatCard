//
// Copyright (C) 2022-2023 QuasarApp.
// Distributed under the GPLv3 software license, see the accompanying
// Everyone is permitted to copy and distribute verbatim copies
// of this license document, but changing it is not allowed.
//


#ifndef IMAGEBACKGROUNDSMODEL_H
#define IMAGEBACKGROUNDSMODEL_H


#include "imagesstoragemodel.h"

namespace RC {

/**
 * @brief The ImageBackgroundsModel class contains background images
 */
class ImageBackgroundsModel: public ImagesStorageModel
{
public:
    ImageBackgroundsModel();
    QStringList initImages() const override;

};
}
#endif // IMAGEBACKGROUNDSMODEL_H
