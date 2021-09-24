#include "testdatatransfer.h"
#include "CheatCard/database.h"

TestDataTransfer::TestDataTransfer(RC::IConnectorBackEnd::Mode mode,
                                   QSharedPointer<RC::DataBase> dataBase):
    RC::IConnectorBackEnd(dataBase->db()) {

    _dataBase = dataBase;
    setMode(mode);

    connect(this, &TestDataTransfer::sigSessionWasFinshed,
            this, &TestDataTransfer::handleWasFinished, Qt::QueuedConnection);


    connect(this, &TestDataTransfer::sigSessionWasBegin,
            this, &TestDataTransfer::handleWasBegin);
}

void TestDataTransfer::addTestConnection(TestDataTransferSocket *target) {
    connectionReceived(target);
}

bool TestDataTransfer::isFinished() const {
    return _finished;
}

RC::IConnectorBackEnd::Status TestDataTransfer::finishedResult() {
    return _lastFinishedResult;
}

int TestDataTransfer::getPurchasesCount(unsigned int user,
                                        unsigned int card) {
    return IConnectorBackEnd::getPurchasesCount(user, card);
}

bool TestDataTransfer::listen(IConnectorBackEnd::Mode mode) {
    setMode(mode);
    return true;
}

bool TestDataTransfer::close() {
    return true;
}

void TestDataTransfer::handleWasFinished(Status err) {
    _lastFinishedResult = err;
    _finished = true;
}

void TestDataTransfer::handleWasBegin() {
    _finished = false;
}
