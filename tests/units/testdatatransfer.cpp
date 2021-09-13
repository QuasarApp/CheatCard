#include "testdatatransfer.h"
#include "RegularCustomer/database.h"

TestDataTransfer::TestDataTransfer(RC::IConnectorBackEnd::Mode mode,
                                   QSharedPointer<RC::DataBase> dataBase):
    RC::IConnectorBackEnd(dataBase->db()) {

    _dataBase = dataBase;
    setMode(mode);
}

void TestDataTransfer::addTestConnection(TestDataTransferSocket *target) {
    connectionReceived(target);
}

bool TestDataTransfer::listen(IConnectorBackEnd::Mode mode) {
    setMode(mode);
    return true;
}

bool TestDataTransfer::close() {
    return true;
}
