//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "db.h"
#include "dbv1.h"

namespace RC {

namespace DB {

bool init() {
    initCheatCardDBResources();
    return true;
}

QSharedPointer<Interfaces::iDB> makeDb(int version,
                                       const QString& name,
                                       const QString& backUpLocation,
                                       const QString& customDBSources) {
    switch (version) {
    case 1: {
        return QSharedPointer<DBv1>::create(name, backUpLocation, customDBSources);
    }
    }

    return nullptr;
}

}
}
