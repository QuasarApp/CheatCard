//
// Copyright (C) 2022-2023 QuasarApp.
// Distributed under the GPLv3 software license, see the accompanying
// Everyone is permitted to copy and distribute verbatim copies
// of this license document, but changing it is not allowed.
//


#include "imagelogomodel.h"

#include <QDir>

namespace RC {

ImageLogoModel::ImageLogoModel() {

}

QStringList ImageLogoModel::initImages() const {
#define r_prefix QString(":/images/private/resources/")

    QDir searcher(r_prefix + "/Icons");
    auto list = searcher.entryInfoList( QDir::Files, QDir::SortFlag::Name);

    QStringList tmpList;
    for (const auto &item : qAsConst(list)) {
        tmpList.push_back("qrc" + item.absoluteFilePath());
    }

    return tmpList;

}

}
