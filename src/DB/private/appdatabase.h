//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef APP_DATABASE_H
#define APP_DATABASE_H

#include <database.h>
#include "isqldb.h"

namespace RC {
namespace DB {


/**
 * @brief The DataBase class This is main databae object.
 */
class AppDataBase: public QH::DataBase
{
    Q_OBJECT
public:
    AppDataBase(const QString& name = "", const QString& backUpLocation = "",
                const QString& customSources = "");
    QH::ISqlDB *db() const;
    QString backUp(int version, QString path = "") const;

    QString localFilePath() const;

    // DataBaseNode interface
    const QString &backUpPath() const;
    void setBackUpPath(const QString &newBackUpPath);

protected:
    QStringList SQLSources() const override;
    void localdbPatches();
    void onBeforeDBUpgrade(int currentVerion, int tergetVersion) const override;

private:
    QString _backUpPath;
    QString _customSqlSources;

};

}
}
#endif // APP_
