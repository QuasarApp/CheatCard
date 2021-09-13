#ifndef TESTDATATRANSFERSOCKET_H
#define TESTDATATRANSFERSOCKET_H
#include "RegularCustomer/itargetnode.h"

class TestDataTransferSocket: public RC::ITargetNode
{
public:
    TestDataTransferSocket(TestDataTransferSocket * another);
    bool sendMessage(const QByteArray &array);
    void close();

    TestDataTransferSocket *getAnother() const;
    void setAnother(TestDataTransferSocket *newAnother);

private:
    TestDataTransferSocket * another = nullptr;
};

#endif // TESTDATATRANSFERSOCKET_H
