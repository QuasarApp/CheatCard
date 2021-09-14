#ifndef TESTDATATRANSFERSOCKET_H
#define TESTDATATRANSFERSOCKET_H
#include "RegularCustomer/itargetnode.h"

class TestDataTransferSocket: public RC::ITargetNode
{
    Q_OBJECT
public:
    TestDataTransferSocket(TestDataTransferSocket * another);
    bool sendMessage(const QByteArray &array);
    void close();

    TestDataTransferSocket *getAnother() const;
    void setAnother(TestDataTransferSocket *newAnother);

private:

    void messageReceived(const QByteArray &data);

    TestDataTransferSocket * another = nullptr;
};

#endif // TESTDATATRANSFERSOCKET_H
