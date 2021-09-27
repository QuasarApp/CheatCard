//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "waitconnectionmodel.h"

#include <QTimer>

namespace RC {

WaitConnectionModel::WaitConnectionModel() {
    _timer = new QTimer(this);

    _timer->setInterval(1000);

    setTimeOut(waitTime());

    connect(_timer, &QTimer::timeout,
            this , &WaitConnectionModel::handleTick);

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

int WaitConnectionModel::waitTime() const {
    return 30;
}

int WaitConnectionModel::timeOut() const {
    return _timeOut;
}

void WaitConnectionModel::setTimeOut(int newTimeOut) {
    if (_timeOut == newTimeOut)
        return;
    _timeOut = newTimeOut;
    emit timeOutChanged();
}

void WaitConnectionModel::begin() {

    setTimeOut(waitTime());
    _timer->start();
    setWaintForCnnect(true);
    emit purchaseTaskCompleted(purchaseCount(), _card);
}

void WaitConnectionModel::cancel() {
    _timer->stop();
    setTimeOut(waitTime());
    setWaintForCnnect(false);
    emit purchaseTaskCanceled();
}

void WaitConnectionModel::handleTick() {
    setTimeOut(timeOut() - 1);

    if(timeOut() <= 0) {
        cancel();
    }
}

void WaitConnectionModel::setWaintForCnnect(bool val) {
    if (_waitForConnect == val)
        return;

    _waitForConnect = val;
    emit waitForConnectChanged();
}

bool WaitConnectionModel::waitForConnect() const {
    return _waitForConnect;
}

void WaitConnectionModel::handlePurchaseTaskFinished() {
    setWaintForCnnect(false);
    emit purchaseTaskFinished();
}

}
