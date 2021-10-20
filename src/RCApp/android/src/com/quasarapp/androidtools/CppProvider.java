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
    providerContext = this;
}

public static void getPremium() {
    if (providerContext != null) {
        providerContext.getPremiumP();
    }
}

public void getPremiumP() {
    billingProcessor.startPurchase();
}

private BillingProcessor billingProcessor;
private static CppProvider providerContext;

}
