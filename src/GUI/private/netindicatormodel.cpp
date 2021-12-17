//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "netindicatormodel.h"
#include "qmlnotifyservice.h"


namespace RC {

NetIndicatorModel::NetIndicatorModel() {

}

bool NetIndicatorModel::getEnableNetwork() const {
    return enableNetwork;
}

void NetIndicatorModel::setEnableNetwork(bool enable) {
    if (enableNetwork == enable)
        return;
    enableNetwork = enable;
    emit enableNetworkChanged();
}

void NetIndicatorModel::handleEndaleNetworkChanged(bool modeNetwork) {

    auto service = QmlNotificationService::NotificationService::getService();

    service->setNotify(tr("Oops. Error code: "),
                       tr("No internet connection."),
                       "", QmlNotificationService::NotificationData::Warning);

    if (!modeNetwork) {
        setEnableNetwork(false);
    }

}

}

