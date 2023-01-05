//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef CARDPROXYMODEL_H
#define CARDPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "icardlistmodel.h"

namespace RC {

class CardProxyModel: public QSortFilterProxyModel, public iCardListModel
{
    Q_OBJECT

public:
    CardProxyModel();

    Q_INVOKABLE void removeCard(int cardId) override;
    Q_INVOKABLE void activateCard(int cardId) override;
    Q_INVOKABLE void activateCardByIndex(int index) override;

protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

}
#endif // CARDPROXYMODEL_H
