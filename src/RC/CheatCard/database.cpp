//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
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
}
