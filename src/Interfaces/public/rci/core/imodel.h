//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef IMODEL_H
#define IMODEL_H

#include <rci/global.h>

#include <QSharedPointer>

namespace RC {
namespace Interfaces {

class iDB;
class iModelsStorage;

/**
 * @brief The iModel class is main model interface. All models shold be initialized after create. See the init method.
 */
class CHEATCARD_INTERFACES_EXPORT iModel
{
public:

    /**
     * @brief init This method initialize the model.
     * @param db This is database interface
     * @param global This is global storage model interface.
     * @param customId This is custom id
     */
    virtual void init(const QSharedPointer<iDB>& db,
                      const QSharedPointer<iModelsStorage>& global) = 0;
};

}
}
#endif // IMODEL_H
