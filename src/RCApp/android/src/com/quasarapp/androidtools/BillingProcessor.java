/*
 * Copyright (C) 2021-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/


package com.quasarapp.androidtools;

import android.app.Activity;

import com.android.billingclient.api.BillingClient;
import com.android.billingclient.api.BillingClientStateListener;
import com.android.billingclient.api.BillingFlowParams;
import com.android.billingclient.api.Purchase;
import com.android.billingclient.api.PurchasesUpdatedListener;
import com.android.billingclient.api.SkuDetails;
import com.android.billingclient.api.SkuDetailsParams;
import com.android.billingclient.api.SkuDetailsResponseListener;
import com.android.billingclient.api.BillingResult;
import com.android.billingclient.api.PurchasesResponseListener;
import com.android.billingclient.api.Purchase.PurchaseState;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


public class BillingProcessor {

    public BillingProcessor(Activity activityContext) {
        m_acrivityContext = activityContext;
    }

    public void initBilling() {

        billingClient = BillingClient.newBuilder(m_acrivityContext).setListener(new PurchasesUpdatedListener() {
            @Override
            public void onPurchasesUpdated(BillingResult billingResult, List<Purchase> purchases) {
                workWithIncomingData(billingResult, purchases);
            }
        }).enablePendingPurchases().build();

        billingClient.startConnection(new BillingClientStateListener() {
            @Override
            public void onBillingServiceDisconnected() {
                //you can restart your client here.
            }

            @Override
            public void onBillingSetupFinished(BillingResult billingResult) {
                if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK){
                    //here billing Client is ready to be used.

                    querySkuDetails();
                }
            }
        });
    }

    public void setCppProvider(CppProvider provider) {
        m_provider = provider;

    }

    private void querySkuDetails() {
        SkuDetailsParams.Builder skuDetailsParamsBuilder
                = SkuDetailsParams.newBuilder();
        ArrayList<String> list = new ArrayList<>();
        list.add(inAppItem1);
        skuDetailsParamsBuilder.setSkusList(list);
        skuDetailsParamsBuilder.setType(BillingClient.SkuType.SUBS);
        billingClient.querySkuDetailsAsync(skuDetailsParamsBuilder.build(), new SkuDetailsResponseListener() {
                    @Override
                    public void onSkuDetailsResponse(BillingResult billingResult, List<SkuDetails> skuDetailsList) {

                        if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK){

                            for (SkuDetails skuDetails : skuDetailsList) {
                                mSkuDetailsMap.put(skuDetails.getSku(), skuDetails);
                            }

                            RestoringPurchases();
                        }
                    }
                });
    }

    public void startPurchase() {

        if (mSkuDetailsMap.size() > 0) {

            BillingFlowParams billingFlowParams = BillingFlowParams.newBuilder()
                    .setSkuDetails(mSkuDetailsMap.get(inAppItem1))
                    .build();

            billingClient.launchBillingFlow(m_acrivityContext, billingFlowParams);
        }
    }

    protected void RestoringPurchases(){
        //To Query you have to provide skuType which is "BillingClient.SkuType.INAPP" or "BillingClient.SkuType.SUBS"
        billingClient.queryPurchasesAsync(BillingClient.SkuType.SUBS,
        new PurchasesResponseListener() {
            @Override
            public void onQueryPurchasesResponse(BillingResult billingResult,
                                                 List<Purchase> purchases) {
                //here you can process your purchases.
                workWithIncomingData(billingResult, purchases);
            }
        });
    }

    private void workWithIncomingData(BillingResult billingResult, List<Purchase> purchases) {
        if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK) {

            //here you can process the new purchases.
            for (Purchase purchase : purchases) {

                if (purchase.getPurchaseState() == PurchaseState.UNSPECIFIED_STATE) {
                    System.out.println("failed to check realy state of purchase");
                    continue;
                }

                if (purchase.getPurchaseState() == PurchaseState.PENDING) {
                    System.out.println("purchase still pending");
                    continue;
                }

                if (m_provider != null) {
                    ArrayList<String> list = purchase.getSkus();
                    for (String id : list) {
                        m_provider.sendPurchaseToApp(id ,purchase.getPurchaseToken());
                    }
                }
            }
        }
    }

    static final String inAppItem1 = "seller";
    private Map<String, SkuDetails> mSkuDetailsMap = new HashMap<>();

    private BillingClient billingClient;
    private Activity m_acrivityContext;
    private CppProvider m_provider;

}
