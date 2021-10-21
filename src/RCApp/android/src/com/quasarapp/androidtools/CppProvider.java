//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

package com.quasarapp.androidtools;

import android.app.Activity;
import com.quasarapp.androidtools.BillingProcessor;

public class CppProvider {

public CppProvider(BillingProcessor billing) {
    billingProcessor = billing;
    billingProcessor.setCppProvider(this);
    providerContext = this;
}

public void getPremiumP() {
    billingProcessor.startPurchase();
}

public void sendPurchaseToApp(String id, String token) {
    purchaseReceived(id, token);
}

public void initBillingP() {
    if (billingProcessor != null) {
        billingProcessor.initBilling();
    }
}

// Cpp Code
private static native void purchaseReceived(String id, String token);

// Java code
private static void getPremium() {
    if (providerContext != null) {
        providerContext.getPremiumP();
    }
}

private static void initBilling() {
    if (providerContext != null) {
        providerContext.initBillingP();
    }
}

private BillingProcessor billingProcessor;
private static CppProvider providerContext;

}
