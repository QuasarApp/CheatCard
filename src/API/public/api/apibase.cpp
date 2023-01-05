//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "apibase.h"
#include <qaglobalutils.h>

namespace RC {

namespace API {

APIBase::APIBase(const QSharedPointer<Interfaces::iDB> &dataBase, QH::AbstractNode *mainNode): QH::iParser(mainNode) {
    _db = dataBase;
    debug_assert(_db, "You try initialize the api object with invalid database.");
}

QSharedPointer<Interfaces::iDB> APIBase::db() const {
    return _db;
}

QString APIBase::parserId() const {
    return API_BASE_PARSE_IS;
}

}
}
