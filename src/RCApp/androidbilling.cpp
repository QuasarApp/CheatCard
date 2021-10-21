//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "androidbilling.h"
#include "javaprovider.h"



#ifdef Q_OS_ANDROID
#include <QAndroidJniEnvironment>

AndroidBilling::AndroidBilling() {

    _javaProvider = JavaProvider::instance();

    connect(_javaProvider, &JavaProvider::sigPurchase,
            this, &AndroidBilling::handlePurchaseReceived);

}

void AndroidBilling::init() {
    _javaProvider->initBilling();
}

void AndroidBilling::becomeSeller() {
    _javaProvider->getPremium();
}

void AndroidBilling::handlePurchaseReceived(QString id, QString token) {
    RC::Purchase purchase;
    purchase.id = id;
    purchase.token = token;
    emit sigPurchaseReceived(purchase);
}

#endif
