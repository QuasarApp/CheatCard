//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <rci/core/imodel.h>


namespace RC {

/**
 * @brief The BaseModel class is base model for all models of the app
 */
class BaseModel: public Interfaces::iModel
{
public:
    BaseModel();

    void init(const QSharedPointer<Interfaces::iDB> &db,
              const QSharedPointer<Interfaces::iModelsStorage> &storage) override;

    /**
     * @brief db return current database object of this node.
     * @return current database object of this node.
     */
    const QSharedPointer<Interfaces::iDB>& db() const;

    /**
     * @brief storage This method return current model's storage object of this node.
     * @return current global scoupe object of this node.
     */
    const QSharedPointer<Interfaces::iModelsStorage>& storage() const;

private:
    QSharedPointer<Interfaces::iDB> _db;
    QSharedPointer<Interfaces::iModelsStorage> _storage;
};

}
#endif // BASEMODEL_H
