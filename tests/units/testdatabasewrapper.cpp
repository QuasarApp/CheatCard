#include "testdatabasewrapper.h"

TestDataBaseWrapper::TestDataBaseWrapper(const QString &name, const QString& dbPath):
    RC::DataBase(name) {
    _dbPath = dbPath;
}

QStringList TestDataBaseWrapper::SQLSources() const {
    if (_dbPath.size())
        return {_dbPath};

    return RC::DataBase::SQLSources();
}
