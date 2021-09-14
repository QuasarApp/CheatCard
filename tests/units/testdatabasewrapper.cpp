#include "testdatabasewrapper.h"

TestDataBaseWrapper::TestDataBaseWrapper(const QString &name):
    RC::DataBase(name) {

}

QStringList TestDataBaseWrapper::SQLSources() const {
    return {":/sql/units/sql/TestSallerDb.sql"};
}
