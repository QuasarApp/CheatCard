//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef ITEMSMODEL_H
#define ITEMSMODEL_H

#include <QStringListModel>

namespace RC {

class ItemsModel : public QStringListModel
{
    Q_OBJECT
public:
    ItemsModel();
    Q_INVOKABLE void addCustomItem(const QString& path);
};
}
#endif // ITEMSMODEL_H
