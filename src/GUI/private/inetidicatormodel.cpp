//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "inetidicatormodel.h"

namespace RC {

InetIdicatorModel::InetIdicatorModel() {

}

bool InetIdicatorModel::getEnableNetwork() const {
    return enableNetwork;
}

void InetIdicatorModel::setEnableNetwork(bool enable) {
    if (enableNetwork == enable)
        return;
    enableNetwork = enable;
    emit EnableNetworkChanged();
}

}
