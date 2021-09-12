#include "testdatatransfer.h"

TestDataTransfer::TestDataTransfer(RC::IConnectorBackEnd::Mode mode) {
    setMode(mode);
}

void TestDataTransfer::addTestConnection(const TestDataTransfer &target) {

}

bool TestDataTransfer::listen(IConnectorBackEnd::Mode mode) {
    setMode(mode);
    return true;
}

bool TestDataTransfer::close() {
    return true;
}
