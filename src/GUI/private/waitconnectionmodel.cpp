//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "iplatformtools.h"
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

    emit purchaseTaskCompleted(purchaseCount(), _card, _extraData);
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

    qDebug() << "_allowScreenDim" << _allowScreenDim;

    if (_allowScreenDim) {
        qDebug() << "clearFlags" << _allowScreenDim;
    } else {
        qDebug() << "addFlags" << _allowScreenDim;
    }

    emit allowScreenDimChanged();
}

}
