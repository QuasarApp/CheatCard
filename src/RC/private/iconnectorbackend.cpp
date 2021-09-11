//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "iconnectorbackend.h"

namespace RC {
IConnectorBackEnd::IConnectorBackEnd()
{

}

bool IConnectorBackEnd::start(Mode mode) {
    return listen(mode);
}

bool IConnectorBackEnd::stop() {
    return close();
}

void IConnectorBackEnd::receiveMessage(const QByteArray &message) {

}

bool IConnectorBackEnd::sendMessage(const QByteArray &message) {

}

void IConnectorBackEnd::connectionReceived(const QByteArray &id) {

}

void IConnectorBackEnd::connectionLost(const QByteArray &id) {

}

}

