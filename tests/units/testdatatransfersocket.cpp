#include "testdatatransfersocket.h"

TestDataTransferSocket::TestDataTransferSocket(TestDataTransferSocket *another) {
    this->another = another;
}

TestDataTransferSocket::~TestDataTransferSocket() {
    close();
}

bool TestDataTransferSocket::sendMessage(const QByteArray &array) {
    if (another) {
        another->messageReceived(array);
    }

    return another;
}

void TestDataTransferSocket::close() {
    if (another) {
        auto tmp = another;
        another = nullptr;
        tmp->close();
        emit sigConnectionClosed(this);
    }
}

TestDataTransferSocket *TestDataTransferSocket::getAnother() const {
    return another;
}

void TestDataTransferSocket::setAnother(TestDataTransferSocket *newAnother) {
    another = newAnother;
}

void TestDataTransferSocket::messageReceived(const QByteArray& data) {
    QByteArray buf;
    buf.append(data);
    emit sigMessageReceived(buf);
}
