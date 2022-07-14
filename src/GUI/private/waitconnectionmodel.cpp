//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "iplatformtools.h"
#include "waitconfirmmodel.h"
#include "waitconnectionmodel.h"
#include <QQmlEngine>

#include <QTimer>
#include <qmlnotifyservice.h>

#include <CheatCard/api/api0/session.h>
#include <CheatCard/api/api0/userheader.h>

namespace RC {

WaitConnectionModel::WaitConnectionModel() {
    _waitModel = new WaitConfirmModel();
    QQmlEngine::setObjectOwnership(_waitModel, QQmlEngine::CppOwnership);
}

WaitConnectionModel::~WaitConnectionModel() {
    delete _waitModel;
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

    auto header = QSharedPointer<API::UserHeader>::create();
    header->fromBytes(QByteArray::fromHex(_extraData.toLatin1()));

    if (!header->isValid()) {
        return;
    }

    if (!_waitModel->wait(header->getSessionId())) {
        return;
    }

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

void WaitConnectionModel::handleSessionServerResult(QSharedPointer<RC::API::Session> session,
                                                    bool succesed) {


    _waitModel->confirm(session->getSessionId(), succesed);

    if (!succesed) {
        auto service = QmlNotificationService::NotificationService::getService();
        service->setNotify(tr("We Have trouble"),
                           tr("Failed to issue a bonus or stamp."
                              " Maybe your local data is deprecated, "
                              " we already update your local data. "
                              " Please try again make issue a bonus or stamp."),
                           "", QmlNotificationService::NotificationData::Error);

    }
}

QObject *WaitConnectionModel::waitModel() const {
    return _waitModel;
}

}
