#ifndef TESTDATATRANSFER_H
#define TESTDATATRANSFER_H

#include <RegularCustomer/iconnectorbackend.h>

class TestDataTransfer: public RC::IConnectorBackEnd {
    Q_OBJECT
public:
    TestDataTransfer(RC::IConnectorBackEnd::Mode mode);

    void addTestConnection(const TestDataTransfer& target);
    void removeTestConnection();
    // IConnectorBackEnd interface
protected:
    bool listen(RC::IConnectorBackEnd::Mode mode) override;
    bool close() override;
};

#endif // TESTDATATRANSFER_H
