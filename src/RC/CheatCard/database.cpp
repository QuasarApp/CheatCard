//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "database.h"
namespace RC {

DataBase::DataBase(const QString &name) {
    if (name.size())
        setLocalNodeName(name);
}

QH::ISqlDBCache *DataBase::db() const {
    return QH::DataBaseNode::db();
}

QString DataBase::localFilePath() const {
    return dbLocation();
}

QStringList DataBase::SQLSources() const {
    return QH::DataBaseNode::SQLSources() << ":/DataBase/private/sql/DataBase.sql";
}

// See https://quasarapp.ddns.net:3031/docs/QuasarApp/Heart/latest/classQH_1_1DataBaseNode.html#a9e2969af3bd4e6b49b80820000aef108
QH::DBPatchMap DataBase::dbPatches() const {


    QH::DBPatchMap result;

    result += [](const QH::iObjectProvider* database) -> bool {
        // Some code for update from 0 to 1
        // Example: return database->doSql(patch.sql);
        Q_UNUSED(database);
        return true;
    };

    // See task #130 https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/130
    result += [](const QH::iObjectProvider* database) -> bool {
        return database->doSql(":/DataBase/private/sql/SQLPatch_1.sql");
    };

    // See task #201 https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/201
    result += [](const QH::iObjectProvider* database) -> bool {
        return database->doSql(":/DataBase/private/sql/SQLPatch_3.sql");
    };

    return result;
}
}
