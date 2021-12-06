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
#include <QDebug>

namespace RC {

AndroidPlatformTools::AndroidPlatformTools() {

}

void AndroidPlatformTools::setScreanDim(bool enable) const {
    QAndroidJniObject activity = QtAndroid::androidActivity();
    if (activity.isValid()) {
        const int FLAG_KEEP_SCREEN_ON = 128;

        if (enable) {
            activity.callMethod<void>("clearWindowFlagAsync", "(I)V", FLAG_KEEP_SCREEN_ON);
        } else {
            activity.callMethod<void>("addWindowFlagAsync", "(I)V", FLAG_KEEP_SCREEN_ON);
        }
    }
}
}
#endif
