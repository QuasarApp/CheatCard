//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "waitconfirmmodel.h"
#include <qmlnotifyservice.h>

namespace RC {

WaitConfirmModel::WaitConfirmModel() {
    connect(&_timer, &QTimer::timeout,
            this, &WaitConfirmModel::handleTimeOut);
}

bool WaitConfirmModel::wait(long long waitFor) {
    if (_notConfirmedSessions) {
        auto service = QmlNotificationService::NotificationService::getService();
        service->setNotify(tr("Not so fast!"),
                           tr("Please wait for finished of a previous request"),
                           "", QmlNotificationService::NotificationData::Warning);
        return false;
    }

    _notConfirmedSessions = waitFor;
    setWaitConfirm(true);
    _timer.start(10000);

    _notConfirmedSessions = waitFor;

    return true;
}

void WaitConfirmModel::confirm(long long confirmedId, bool success) {

    auto service = QmlNotificationService::NotificationService::getService();

    if (success) {
        if (confirmedId == _notConfirmedSessions) {
            disbaleWaiting();
        }

    } else {

        if (confirmedId == _notConfirmedSessions) {
            disbaleWaiting();

            service->setNotify(tr("We Have trouble"),
                               tr("Failed to issue a bonus or stamp."
                                  " Maybe your local data is deprecated, "
                                  " we already update your local data. "
                                  " Please try again make issue a bonus or stamp."),
                               "", QmlNotificationService::NotificationData::Error);
        }
    }
}

bool WaitConfirmModel::waitConfirm() const {
    return _waitConfirm;
}

void WaitConfirmModel::handleTimeOut() {
    if (_notConfirmedSessions) {
        disbaleWaiting();
        auto service = QmlNotificationService::NotificationService::getService();

        service->setNotify(tr("Server not responds"),
                           tr("Hmm, maybe you have a super slow internet connection... Try again"),
                           "", QmlNotificationService::NotificationData::Error);
    }

    _timer.stop();
}

void WaitConfirmModel::disbaleWaiting() {
    _notConfirmedSessions = 0;
    setWaitConfirm(false);
    _timer.stop();
}

void WaitConfirmModel::setWaitConfirm(bool newWitConfirm) {
    if (_waitConfirm == newWitConfirm)
        return;
    _waitConfirm = newWitConfirm;
    emit waitConfirmChanged();
}

}
