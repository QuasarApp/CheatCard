//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "iplatformtools.h"
#include "waitconnectionmodel.h"

#include <QTimer>
#include <qmlnotifyservice.h>

#include <CheatCard/api/api0/session.h>
#include <CheatCard/api/api0/userheader.h>

namespace RC {

WaitConnectionModel::WaitConnectionModel() {
    connect(&_timer, &QTimer::timeout,
            this, &WaitConnectionModel::handleTimeOut);
}

QObject* WaitConnectionModel::card() const {
    return _card.data();
}

void WaitConnectionModel::setCard(const QSharedPointer<CardModel> &newCard) {

    if (_card == newCard)
        return;
    _card = newCard;
    emit cardChanged();
}

int WaitConnectionModel::purchaseCount() const {
    return _purchaseCount;
}

void WaitConnectionModel::setPurchaseCount(int newPurchaseCount) {
    if (_purchaseCount == newPurchaseCount)
        return;
    _purchaseCount = newPurchaseCount;
    emit purchaseCountChanged();
}

void WaitConnectionModel::begin() {

    if (_notConfirmedSessions) {
        // please wait for finished of a previous request
        return;
    }

    auto header = QSharedPointer<API::UserHeader>::create();
    header->fromBytes(QByteArray::fromHex(_extraData.toLatin1()));

    if (!header->isValid()) {
        return;
    }

    _notConfirmedSessions = header->getSessionId();
    setWaitConfirm(true);
    _timer.start(10000);

    emit purchaseTaskCompleted(purchaseCount(), _card, header);
}

void WaitConnectionModel::cancel() {
}

const QString &WaitConnectionModel::extraData() const {
    return _extraData;
}

void WaitConnectionModel::setExtraData(const QString &newExtraData) {
    if (_extraData == newExtraData)
        return;
    _extraData = newExtraData;
    emit extraDataChanged();
}

bool WaitConnectionModel::allowScreenDim() const {
    return _allowScreenDim;
}

void WaitConnectionModel::setAllowScreenDim(bool newAllowScreenDim) {
    if (_allowScreenDim == newAllowScreenDim)
        return;
    _allowScreenDim = newAllowScreenDim;

    IPlatformTools::instance()->setScreanDim(newAllowScreenDim);

    emit allowScreenDimChanged();
}

void WaitConnectionModel::handleTimeOut() {

    if (_notConfirmedSessions) {
        disbaleWaiting();
        auto service = QmlNotificationService::NotificationService::getService();

        service->setNotify(tr("Server not responds"),
                           tr("Hmm, maybe you have a super slow internet connection... Try again"),
                           "", QmlNotificationService::NotificationData::Error);
    }

    _timer.stop();
}

bool WaitConnectionModel::waitConfirm() const {
    return _waitConfirm;
}

void WaitConnectionModel::setWaitConfirm(bool newWitConfirm) {
    if (_waitConfirm == newWitConfirm)
        return;
    _waitConfirm = newWitConfirm;
    emit waitConfirmChanged();
}

void RC::WaitConnectionModel::disbaleWaiting() {
    _notConfirmedSessions = 0;
    setWaitConfirm(false);
    _timer.stop();
}

void WaitConnectionModel::handleSessionServerResult(QSharedPointer<RC::API::Session> session,
                                                    bool succesed) {

    auto service = QmlNotificationService::NotificationService::getService();

    if (succesed) {
        if (session->getSessionId() == _notConfirmedSessions) {
            disbaleWaiting();
        }

    } else {

        if (session->getSessionId() == _notConfirmedSessions) {
            disbaleWaiting();
        }

        service->setNotify(tr("We Have trouble"),
                           tr("Failed to issue a bonus or stamp."
                              " Maybe your local data is deprecated, "
                              " we already update your local data. "
                              " Please try again make issue a bonus or stamp."),
                           "", QmlNotificationService::NotificationData::Error);

    }
}

}
