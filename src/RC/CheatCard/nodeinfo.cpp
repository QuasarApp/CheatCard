//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "nodeinfo.h"

#include <hostaddress.h>
namespace RC {

NodeInfo::NodeInfo(QAbstractSocket *sct, const QH::HostAddress *address):
    QH::AbstractNodeInfo(sct, address) {

}

unsigned long long NodeInfo::token() const {
    return _token;
}

void NodeInfo::setToken(unsigned long long newToken) {
    _token = newToken;
    _fVersionReceived = false;
}

int NodeInfo::version() const {
    return _version;
}

void NodeInfo::setVersion(int newVersion) {
    _version = newVersion;
    _fVersionReceived = true;
}

bool NodeInfo::confirmData() const {
    return _fVersionReceived;
}
}
