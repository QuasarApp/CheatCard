//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef STATISTICKLISTPROXYMODEL_H
#define STATISTICKLISTPROXYMODEL_H

#include <QSortFilterProxyModel>

namespace RC {

class StatistickListProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    StatistickListProxyModel(QObject* parent = nullptr);

    // QSortFilterProxyModel interface
protected:
    bool lessThan(const QModelIndex &source_left,
                  const QModelIndex &source_right) const override;
};

}

#endif // STATISTICKLISTPROXYMODEL_H
