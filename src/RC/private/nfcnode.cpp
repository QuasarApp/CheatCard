//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "nfcnode.h"

#include <QtNfc/qndefmessage.h>
#include <QtNfc/QNdefRecord>
#include <quasarapp.h>
#include <qmlnotifyservice.h>

namespace RC {

NFCNode::NFCNode(QNearFieldTarget * source) {
    d_ptr = source;
    if (!d_ptr->setKeepConnection(true)) {
        QuasarAppUtils::Params::log("The device not support long time connect!",
                                    QuasarAppUtils::Error);
    }

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
    record.setType("data/cheatcard");

    QNdefMessage message;
    message.push_back(record);

    auto id = d_ptr->writeNdefMessages(QList<QNdefMessage>() << message);

    if (!id.isValid()) {
        QuasarAppUtils::Params::log("Failed To send message by NFC",
                                    QuasarAppUtils::Error);
        auto service = QmlNotificationService::NotificationService::getService();
        service->setNotify("NFC Error", "Failed To send message by NFC");
        return false;
    }

    return true;
}

void NFCNode::close() {
    emit sigConnectionClosed(this);
    d_ptr->disconnect();
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
