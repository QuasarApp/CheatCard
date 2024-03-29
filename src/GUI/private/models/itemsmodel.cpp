//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
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

void ItemsModel::setStringList(const QStringList &strings) {

    QStringListModel::setStringList(QStringList() << "clear" << strings);
    // see imageProvider
    addCustomItem("add");
}
}
