/*
 * Copyright (C) 2021-2021 QuasarApp.
 * Distributed under the lgplv3 software license, see the accompanying
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
*/

package com.quasarapp.androidtools;

import org.qtproject.qt5.android.bindings.QtActivity;
import com.quasarapp.androidtools.BillingProcessor;
import com.quasarapp.androidtools.CppProvider;

import android.os.Bundle;

import android.view.View;
import android.view.Window;

public class MainActivity extends QtActivity
{
@Override
public void onWindowFocusChanged(boolean hasFocus) {
    super.onWindowFocusChanged(hasFocus);


}

@Override
public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    billingProcessor = new BillingProcessor(this);
    cppProvider = new CppProvider(billingProcessor);

}

private void hideSystemUI() {
    // Enables regular immersive mode.
    // For "lean back" mode, remove SYSTEM_UI_FLAG_IMMERSIVE.
    // Or for "sticky immersive," replace it with SYSTEM_UI_FLAG_IMMERSIVE_STICKY
    View decorView = getWindow().getDecorView();
    decorView.setSystemUiVisibility(
            View.SYSTEM_UI_FLAG_IMMERSIVE
            // Set the content to appear under the system bars so that the
            // content doesn't resize when the system bars hide and show.
            | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
            // Hide the nav bar and status bar
            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
            | View.SYSTEM_UI_FLAG_FULLSCREEN);
}

// Shows the system bars by removing all the flags
// except for the ones that make the content appear under the system bars.
private void showSystemUI() {
    View decorView = getWindow().getDecorView();
    decorView.setSystemUiVisibility(
            View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
}

@Override
public void onBackPressed() {
    super.onBackPressed();

}

// see google docs https://developer.android.com/reference/android/view/Window
public void clearWindowFlagAsync(int flag) {


    //perform heavy task here and finally update the UI with result this way -
    runOnUiThread(new Runnable() {
        @Override
        public void run() {
            Window decorView = getWindow();
            decorView.clearFlags(flag);
        }
    });
}

public void addWindowFlagAsync(int flag) {

    //perform heavy task here and finally update the UI with result this way -
    runOnUiThread(new Runnable() {
        @Override
        public void run() {
            Window decorView = getWindow();
            decorView.addFlags(flag);
        }
    });
}

private BillingProcessor billingProcessor;
private CppProvider cppProvider;

}
