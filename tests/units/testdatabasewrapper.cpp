#include "testdatabasewrapper.h"

TestDataBaseWrapper::TestDataBaseWrapper(const QString &name,
                                         const QString& dbPath):
    RC::DataBase(name) {
    _dbPath = dbPath;
}

QStringList TestDataBaseWrapper::SQLSources() const {
    if (_dbPath.size())
        return {_dbPath};

    return RC::DataBase::SQLSources();
}

QVariantMap TestDataBaseWrapper::defaultDbParams() const {
    auto result = RC::DataBase::defaultDbParams();

    result["DBFilePath"] =  APP_DATA_LOCATION + "/" + localNodeName() + "/" + localNodeName() + "_" + DEFAULT_DB_NAME;

    return result;

}
