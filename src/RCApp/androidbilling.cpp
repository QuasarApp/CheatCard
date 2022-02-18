//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "androidbilling.h"
#include "javaprovider.h"



#ifdef Q_OS_ANDROID
#include <QAndroidJniEnvironment>

// Play Market public license key.
static const char* publick_key = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAiooKz+RwMhtk0quCzKVlUza1kgVEYtoNPz9tfn/JCRVH5+PibFPF3uv9Th9ts8ikyr9Yiz13vaVbQO4NQ6Gf0SgqTOpHwYGyDnSZSkPSYkH5KcXpXNVZzkQvrcBU10D8m6QJqnWfnFYsvtt90axO9/yf1Y7EeIvH3+gQ9VkJezclfG3CJb41l4gg2JKqOCHdGBBo0RWNe9mKBKzhSodlXY+3bjXnbYUZOwn1+KxNdiFgzlSLW3jdJ0IuuEe8dmxiWJLP6CW5qed61wzHQwKhsJYlno/d/NfFu6RSBGKFmwPV4gZWDB46pOVJsOFNSd5XhzZjVpNyN1NOT6ws7nd6GQIDAQAB";

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

const char *AndroidBilling::getPublicKey() {
    return publick_key;
}

bool AndroidBilling::isSupported() {
    return true;
}

#endif
