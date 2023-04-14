//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "CheatCard/userheader.h"
#include "iplatformtools.h"
#include "incomemodel.h"
#include <QQmlEngine>

#include <QTimer>
#include <qmlnotifyservice.h>

namespace RC {

IncomeModel::IncomeModel() {
}

IncomeModel::~IncomeModel() {
}

QObject* IncomeModel::card() const {
    return _card.data();
}

void IncomeModel::setCard(const QSharedPointer<CardModel> &newCard) {

    if (_card == newCard)
        return;
    _card = newCard;
    emit cardChanged();
}

int IncomeModel::purchaseCount() const {
    return _purchaseCount;
}

void IncomeModel::setPurchaseCount(int newPurchaseCount) {
    if (_purchaseCount == newPurchaseCount)
        return;
    _purchaseCount = newPurchaseCount;
    emit purchaseCountChanged();
}

void IncomeModel::begin() {

    auto header = QSharedPointer<UserHeader>::create();
    header->fromBytes(QByteArray::fromHex(_extraData.toLatin1()));

    if (!header->isValid()) {
        return;
    }

    emit purchaseTaskCompleted(purchaseCount(), _card, header);
}

void IncomeModel::cancel() {
}

const QString &IncomeModel::extraData() const {
    return _extraData;
}

void IncomeModel::setExtraData(const QString &newExtraData) {
    if (_extraData == newExtraData)
        return;
    _extraData = newExtraData;
    emit extraDataChanged();
}

bool IncomeModel::allowScreenDim() const {
    return _allowScreenDim;
}

void IncomeModel::setAllowScreenDim(bool newAllowScreenDim) {
    if (_allowScreenDim == newAllowScreenDim)
        return;
    _allowScreenDim = newAllowScreenDim;

    IPlatformTools::instance()->setScreanDim(newAllowScreenDim);

    emit allowScreenDimChanged();
}

}
