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

    setTimeOut(10);
    _timer->start();

    emit purchaseTaskCompleted(purchaseCount(), _card);
}

void WaitConnectionModel::handleTick() {
    setTimeOut(timeOut() - 1);

    if(timeOut() <= 0) {
        emit purchaseTaskCanceled();
    }
}
}
