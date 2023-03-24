//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef ITEMSMODEL_H
#define ITEMSMODEL_H

#include "basemodel.h"

#include <QStringListModel>

namespace RC {

class ItemsModel : public QStringListModel, public BaseModel
{
    Q_OBJECT
public:
    ItemsModel();
    Q_INVOKABLE void addCustomItem(const QString& path);
    void setStringList(const QStringList &strings);
};
}
#endif // ITEMSMODEL_H
