#ifndef TESTDATATRANSFER_H
#define TESTDATATRANSFER_H

#include <RegularCustomer/iconnectorbackend.h>
#include <RegularCustomer/database.h>
#include "testdatatransfersocket.h"

class TestDataTransfer: public RC::IConnectorBackEnd {
    Q_OBJECT
public:
    TestDataTransfer(RC::IConnectorBackEnd::Mode mode,
                     QSharedPointer<RC::DataBase> db = QSharedPointer<RC::DataBase>::create());

    void addTestConnection(TestDataTransferSocket *target);
    void removeTestConnection();
    // IConnectorBackEnd interface
protected:
    bool listen(RC::IConnectorBackEnd::Mode mode) override;
    bool close() override;

private:
    QSharedPointer<RC::DataBase> _dataBase = nullptr;
};

#endif // TESTDATATRANSFER_H
