//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef DATABASE_H
#define DATABASE_H

#include "heart.h"
#include "isqldbcache.h"
#include "core_global.h"


namespace RC {

/**
 * @brief The DataBase class This is main databae object.
 */
class CHEATCARD_CORE_EXPORT DataBase: public QH::DataBaseNode
{
    Q_OBJECT
public:
    DataBase(const QString& name = "");
    QH::ISqlDBCache *db() const;

    QString localFilePath() const;

    // DataBaseNode interface
protected:
    QStringList SQLSources() const;
    QH::DBPatchMap dbPatches() const;

};

}
#endif // DATABASE_H
