//
// Copyright (C) 2022-2023 QuasarApp.
// Distributed under the GPLv3 software license, see the accompanying
// Everyone is permitted to copy and distribute verbatim copies
// of this license document, but changing it is not allowed.
//

#ifndef IMAGELOGOMODEL_H
#define IMAGELOGOMODEL_H

#include "imagesstoragemodel.h"

namespace RC {

/**
 * @brief The ImageLogoModel class contails all default logotips
 */
class ImageLogoModel: public ImagesStorageModel
{
public:
    ImageLogoModel();


    QStringList initImages() const override;
};

}
#endif // IMAGELOGOMODEL_H
