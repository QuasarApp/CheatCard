#include "nfcnode.h"

#include <QtNfc/qndefmessage.h>
#include <QtNfc/QNdefRecord>
#include <quasarapp.h>
namespace RC {

NFCNode::NFCNode(QNearFieldTarget * source) {
    d_ptr = source;

    connect(d_ptr, &QNearFieldTarget::ndefMessageRead,
            this, &NFCNode::handleReceiveRawData);

    connect(d_ptr, &QNearFieldTarget::error,
            this, &NFCNode::handleError);

}

NFCNode::~NFCNode() {
    NFCNode::close();
}

bool NFCNode::sendMessage(const QByteArray &array) {
    if (!d_ptr)
        return false;


    QNdefRecord record;
    record.setPayload(array);

    QNdefMessage message;
    message.push_back(record);

    auto id = d_ptr->writeNdefMessages(QList<QNdefMessage>() << message);

    return id.isValid();
}

void NFCNode::close() {
    emit sigConnectionClosed(this);
    d_ptr->deleteLater();
    d_ptr = nullptr;
}

unsigned int NFCNode::nodeId() const {
    return qHash(reinterpret_cast<unsigned long long>(d_ptr));
}

void NFCNode::handleReceiveRawData(const QNdefMessage &message) {
    QByteArray rawData;
    for (const auto &record: message) {
        rawData.append(record.payload());
    }

    emit sigMessageReceived(rawData);
}

void NFCNode::handleError(QNearFieldTarget::Error error,
                          const QNearFieldTarget::RequestId &) {

    QuasarAppUtils::Params::log("NFC Error code :" + QString::number(error),
                                QuasarAppUtils::Error);

    close();

}

}
