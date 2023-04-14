//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "netindicatormodel.h"
#include "notificationservice.h"


namespace RC {

NetIndicatorModel::NetIndicatorModel() {

}

bool NetIndicatorModel::getEnableNetwork() const {
    return _enableNetwork;
}

void NetIndicatorModel::setEnableNetwork(bool enable) {
    if (_enableNetwork == enable)
        return;
    _enableNetwork = enable;
    emit enableNetworkChanged();
}

void NetIndicatorModel::handleEndaleNetworkChanged(bool modeNetwork) {

    if (!modeNetwork) {
        auto service = QmlNotificationService::NotificationService::getService();
        service->setNotify(tr("Oops"),
                           tr("No internet connection."),
                           "", QmlNotificationService::NotificationData::Warning);
    }

    setEnableNetwork(modeNetwork);

}

bool NetIndicatorModel::dataExchanging() const {
    return _dataExchanging;
}

void NetIndicatorModel::setDataExchanging(bool newDataExchanging) {
    if (_dataExchanging == newDataExchanging)
        return;
    _dataExchanging = newDataExchanging;

    emit dataExchangingChanged();
}

}

