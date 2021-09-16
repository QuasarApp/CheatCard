#include "testdatabasewrapper.h"
#include "RegularCustomer/iconnectorbackend.h"
TestDataBaseWrapper::TestDataBaseWrapper(const QString &name, int mode):
    RC::DataBase(name) {
    _mode = mode;
}

QStringList TestDataBaseWrapper::SQLSources() const {
    if (_mode == RC::IConnectorBackEnd::Mode::Saller)
        return {":/sql/units/sql/TestSallerDb.sql"};

    return RC::DataBase::SQLSources();
}
