//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "statisticklistproxymodel.h"
#include "sellerstatisticmodel.h"

namespace RC {

StatistickListProxyModel::StatistickListProxyModel(QObject *parent):
    QSortFilterProxyModel(parent) {
    setSortRole(SellerStatisticModel::SortRole);
    sort(5, Qt::SortOrder::DescendingOrder);

}

bool StatistickListProxyModel::lessThan(const QModelIndex &source_left,
                                        const QModelIndex &source_right) const {

    return QSortFilterProxyModel::lessThan(source_left, source_right);
}

}
