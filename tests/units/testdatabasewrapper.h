#ifndef TESTDATABASEWRAPPER_H
#define TESTDATABASEWRAPPER_H

#include "CheatCard/database.h"

class TestDataBaseWrapper: public RC::DataBase
{
    Q_OBJECT
public:
    TestDataBaseWrapper(const QString& db, const QString &dbPath = "");

    // DataBaseNode interface
protected:
    QStringList SQLSources() const;
private:
    QString _dbPath;
};

#endif // TESTDATABASEWRAPPER_H
