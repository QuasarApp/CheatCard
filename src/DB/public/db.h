//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef DB_H
#define DB_H

#include "rcdb/global.h"
#include "rci/core/idb.h"

inline void initCheatCardDBResources() {
    Q_INIT_RESOURCE(db);
}

namespace RC {

/**
 * @brief DB namespace is base name space for all data base objects of the cheatcard app.
 */
namespace DB {

/**
 * @brief init This method initialize the local database resources for application.
 * @return true if function finished successfull else false.
 */
bool CHEATCARD_DB_EXPORT init();

/**
 * @brief makeDb This is factory method of the dayabase objects.
 * @param name This is name of the database.
 * @param backUpLocation This is location backups.
 * @param version This is required version
 * @param customDBSources This is path to file with custom sql sources for db, This option will may use for tests.
 * @return database object or nullptr object if the @a version not supported.
 */
QSharedPointer<Interfaces::iDB> makeDb(int version, const QString &name = "",
                                       const QString &backUpLocation = "",
                                       const QString& customDBSources = "");

}
}

#endif // DB_H
