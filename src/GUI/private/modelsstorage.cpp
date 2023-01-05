//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "modelsstorage.h"

namespace RC {

ModelsStorage::ModelsStorage(const QSharedPointer<Interfaces::iDB> &db) {
    _db = db;
}

QSharedPointer<Interfaces::iModel> ModelsStorage::get(int id) const {
    return _storage.value(id);
}

QSharedPointer<Interfaces::iDB> ModelsStorage::db() const {
    return _db;
}

}
