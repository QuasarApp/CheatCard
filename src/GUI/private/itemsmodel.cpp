//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "itemsmodel.h"

namespace RC {

ItemsModel::ItemsModel() {

}

void ItemsModel::addCustomItem(const QString &path) {

    int newDataIndex = rowCount();

    insertRow(newDataIndex);
    setData(index(newDataIndex), path);
}
}
