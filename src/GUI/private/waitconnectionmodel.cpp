//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "waitconnectionmodel.h"

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#endif

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

#ifdef Q_OS_ANDROID
QAndroidJniObject activity = QtAndroid::androidActivity();
if (activity.isValid()) {
    QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

    if (window.isValid()) {
        const int FLAG_KEEP_SCREEN_ON = 128;

        // see google docs https://developer.android.com/reference/android/view/Window
        if (_allowScreenDim) {
            window.callMethod<void>("clearFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
        } else {
            window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
        }

    }

    //Clear any possible pending exceptions.
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
    }
}
#endif

    emit allowScreenDimChanged();
}

}
