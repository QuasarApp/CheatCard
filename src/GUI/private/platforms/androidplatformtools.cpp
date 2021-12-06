//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "androidplatformtools.h"

#ifdef Q_OS_ANDROID

#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>

namespace RC {

AndroidPlatformTools::AndroidPlatformTools() {

}

void AndroidPlatformTools::setScreanDim(bool enable) const {
    QAndroidJniObject activity = QtAndroid::androidActivity();
    if (activity.isValid()) {
        QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

        if (window.isValid()) {
            const int FLAG_KEEP_SCREEN_ON = 128;

            // see google docs https://developer.android.com/reference/android/view/Window
            if (enable) {
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
}
}
#endif
