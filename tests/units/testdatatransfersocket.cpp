#include "testdatatransfersocket.h"

TestDataTransferSocket::TestDataTransferSocket(TestDataTransferSocket *another) {
    this->another = another;
}

bool TestDataTransferSocket::sendMessage(const QByteArray &array) {
    if (another) {
        another->messageReceived(array);
    }

    return another;
}

void TestDataTransferSocket::close() {

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
