#ifndef TESTDATATRANSFER_H
#define TESTDATATRANSFER_H

#include <CheatCard/iconnectorbackend.h>
#include <CheatCard/database.h>
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
    Status finishedResult();

    int getPurchasesCount(unsigned int user, unsigned int card);

protected:
    bool listen(RC::IConnectorBackEnd::Mode mode) override;
    bool close() override;

private slots:
    void handleWasFinished(Status err);
    void handleWasBegin();
private:
    QSharedPointer<RC::DataBase> _dataBase = nullptr;
    Status _lastFinishedResult;
    bool _finished = false;
};

#endif // TESTDATATRANSFER_H
