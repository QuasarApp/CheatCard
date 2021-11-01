//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef CARDPROXYMODEL_H
#define CARDPROXYMODEL_H

#include <QSortFilterProxyModel>


namespace RC {

class CardProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT
public:
    CardProxyModel();

    // QSortFilterProxyModel interface
protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

}
#endif // CARDPROXYMODEL_H
