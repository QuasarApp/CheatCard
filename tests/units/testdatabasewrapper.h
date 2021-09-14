#ifndef TESTDATABASEWRAPPER_H
#define TESTDATABASEWRAPPER_H

#include "RegularCustomer/database.h"

class TestDataBaseWrapper: public RC::DataBase
{
    Q_OBJECT
public:
    TestDataBaseWrapper(const QString& name);

    // DataBaseNode interface
protected:
    QStringList SQLSources() const;
};

#endif // TESTDATABASEWRAPPER_H
