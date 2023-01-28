//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "appdatabase.h"
#include <sqldbwriter.h>

#include <customdbrequest.h>
#include <getsinglevalue.h>

#include <QDateTime>
#include <QDir>

#include <pills/invaliduserspill.h>

#include <objects/card.h>
#include <objects/user.h>

#include <rci/rcutils.h>

namespace RC {
namespace DB {

AppDataBase::AppDataBase(const QString &name, const QString &backUpLocation,
                         const QString &customSources) {
    if (name.size())
        setLocalNodeName(name);

    _customSqlSources = customSources;

    setBackUpPath(backUpLocation);
    localdbPatches();
}

QH::ISqlDB *AppDataBase::db() const {
    return QH::DataBase::db();
}

QString AppDataBase::localFilePath() const {
    return dbLocation();
}

QStringList AppDataBase::SQLSources() const {
    if (_customSqlSources.size()) {
        return QH::DataBase::SQLSources() << _customSqlSources;
    }

    return QH::DataBase::SQLSources();
}

void AppDataBase::localdbPatches() {

    addDBPatch({
        0,
        6,
        [](const QH::iObjectProvider* database) -> bool {
            return database->doSql(":/DataBase/private/sql/SQLPatch_6.sql");
        }
    });

    addDBPatch({
        6,
        7,
        [](const QH::iObjectProvider* database) -> bool {
            return database->doSql(":/DataBase/private/sql/SQLPatch_7.sql");
        }
    });
}

void AppDataBase::onBeforeDBUpgrade(int currentVersion, int ) const {
    backUp(currentVersion);
}

QString AppDataBase::backUp(int version, QString path) const {

    if (path.isEmpty()) {
        path = _backUpPath;
    }

    if (path.isEmpty()) {
        return {};
    }

    auto file = path + "/DBv%0_" + QDateTime::currentDateTimeUtc().toString("hh:mm:ss_dd_MM_yyyy") + ".db";
    file = file.arg(version);
    if (db() && db()->writer() &&
            QFile::exists(db()->writer()->databaseLocation())) {

        QDir().mkpath(path);

        if (!QFile::copy(db()->writer()->databaseLocation(), file)) {
            return {};
        }
    }

    return file;
}

const QString &AppDataBase::backUpPath() const {
    return _backUpPath;
}

void AppDataBase::setBackUpPath(const QString &newBackUpPath) {
    _backUpPath = newBackUpPath;
}

}
}
