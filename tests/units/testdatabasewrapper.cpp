#include "testdatabasewrapper.h"

TestDataBaseWrapper::TestDataBaseWrapper(const QString &name, int mode):
    RC::DataBase(name) {
    _mode = mode;
}

QStringList TestDataBaseWrapper::SQLSources() const {
    if (_mode == 1)
        return {":/sql/units/sql/TestSallerDb.sql"};

    return RC::DataBase::SQLSources();
}
