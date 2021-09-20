#ifndef NFCNODE_H
#define NFCNODE_H

#include "CheatCard/itargetnode.h"

#include <QtNfc/QNearFieldTarget>

namespace RC {
class NFCNode final: public RC::ITargetNode
{
    Q_OBJECT
public:
    NFCNode(QNearFieldTarget * source);
    ~NFCNode() override;

    bool sendMessage(const QByteArray &array) override final;
    void close() override final;

    unsigned int nodeId() const override;
private slots:
    void handleReceiveRawData(const QNdefMessage &message);
    void handleError(QNearFieldTarget::Error error,
                     const QNearFieldTarget::RequestId &id);

private:
    QNearFieldTarget *d_ptr = nullptr;
};
}


#endif // NFCNODE_H
