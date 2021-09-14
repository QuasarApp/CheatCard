#include "testdatatransfersocket.h"

TestDataTransferSocket::TestDataTransferSocket(TestDataTransferSocket *another) {
    this->another = another;
}

bool TestDataTransferSocket::sendMessage(const QByteArray &array) {
    if (another) {
        _buffer = array;
        emit another->sigMessageReceived(_buffer);
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
