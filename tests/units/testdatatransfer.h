#ifndef TESTDATATRANSFER_H
#define TESTDATATRANSFER_H

#include <RegularCustomer/iconnectorbackend.h>
#include <RegularCustomer/database.h>
#include "testdatatransfersocket.h"

class TestDataTransfer: public RC::IConnectorBackEnd {
    Q_OBJECT
public:
    TestDataTransfer(RC::IConnectorBackEnd::Mode mode,
                     QSharedPointer<RC::DataBase> db);

    void addTestConnection(TestDataTransferSocket *target);
    void removeTestConnection();
    // IConnectorBackEnd interface

    bool isFinished() const;
    Error finishedResult();

protected:
    bool listen(RC::IConnectorBackEnd::Mode mode) override;
    bool close() override;

private slots:
    void handleWasFinished(Error err);
    void handleWasBegin();
private:
    QSharedPointer<RC::DataBase> _dataBase = nullptr;
    Error _lastFinishedResult;
    bool _finished = false;
};

#endif // TESTDATATRANSFER_H
