//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "imagesstoragemodel.h"

#include <QCryptographicHash>

namespace RC {
ImagesStorageModel::ImagesStorageModel() {

}

ImagesStorageModel::~ImagesStorageModel() {

}

const QString &ImagesStorageModel::getImageByHash(const QByteArray &anyData) {
    return getImageByHash(qHash(anyData));
}

const QString& ImagesStorageModel::getImageByHash(int anyNumber) {

    auto &images = getImages();
    int index = std::abs(anyNumber % images.size());

    return images[index];
}

const QStringList &ImagesStorageModel::getImages() {
    if (_data.isEmpty()) {
        _data = initImages();
    }

    return _data;
}


}
