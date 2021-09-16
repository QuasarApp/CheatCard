#ifndef TESTDATABASEWRAPPER_H
#define TESTDATABASEWRAPPER_H

#include "RegularCustomer/database.h"

class TestDataBaseWrapper: public RC::DataBase
{
    Q_OBJECT
public:
    TestDataBaseWrapper(const QString& name, int mode);

    // DataBaseNode interface
protected:
    QStringList SQLSources() const;
private:
    int _mode = 0;
};

#endif // TESTDATABASEWRAPPER_H
