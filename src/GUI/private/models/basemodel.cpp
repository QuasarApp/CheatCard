//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "basemodel.h"

namespace RC {
BaseModel::BaseModel() {

}

void BaseModel::init(const QSharedPointer<Interfaces::iDB> &db,
                     const QSharedPointer<Interfaces::iModelsStorage> &global) {

    _db = db;
    _storage = global;
}

const QSharedPointer<Interfaces::iDB> & BaseModel::db() const {
    return _db;
}

const QSharedPointer<Interfaces::iModelsStorage> &BaseModel::storage() const {
    return _storage;
}

}
