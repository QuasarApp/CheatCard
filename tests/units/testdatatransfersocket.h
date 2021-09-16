#ifndef TESTDATATRANSFERSOCKET_H
#define TESTDATATRANSFERSOCKET_H
#include "CheatCard/itargetnode.h"

class TestDataTransferSocket: public RC::ITargetNode
{
    Q_OBJECT
public:
    TestDataTransferSocket(TestDataTransferSocket * another);
    ~TestDataTransferSocket() override;
    bool sendMessage(const QByteArray &array) override final;
    void close() override final;

    TestDataTransferSocket *getAnother() const;
    void setAnother(TestDataTransferSocket *newAnother);

private:

    void messageReceived(const QByteArray &data);

    TestDataTransferSocket * another = nullptr;
};

#endif // TESTDATATRANSFERSOCKET_H
