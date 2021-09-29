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

    setWaintForCnnect(true);
    emit purchaseTaskCompleted(purchaseCount(), _card, _extraData);
    handlePurchaseTaskFinished();
}

void WaitConnectionModel::cancel() {
    setWaintForCnnect(false);
    emit purchaseTaskCanceled();
}

void WaitConnectionModel::setWaintForCnnect(bool val) {
    if (_waitForConnect == val)
        return;

    _waitForConnect = val;
    emit waitForConnectChanged();
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

bool WaitConnectionModel::waitForConnect() const {
    return _waitForConnect;
}

void WaitConnectionModel::handlePurchaseTaskFinished() {
    setWaintForCnnect(false);
    emit purchaseTaskFinished();
}

}
