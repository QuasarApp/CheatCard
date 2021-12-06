#ifndef TESTDATABASEWRAPPER_H
#define TESTDATABASEWRAPPER_H

#include "CheatCard/database.h"
#define APP_DATA_LOCATION DEFAULT_DB_PATH

class TestDataBaseWrapper: public RC::DataBase
{
    Q_OBJECT
public:
    TestDataBaseWrapper(const QString& db, const QString &dbPath = "");
    // DataBaseNode interface

protected:
    QStringList SQLSources() const override;
    QVariantMap defaultDbParams() const override;

private:
    QString _dbPath;

};

#endif // TESTDATABASEWRAPPER_H
