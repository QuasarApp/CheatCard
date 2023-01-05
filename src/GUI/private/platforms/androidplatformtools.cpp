//#
//# Copyright (C) 2021-2023 QuasarApp.
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

bool AndroidPlatformTools::getAccessToWriteInStorage() {
    if (QtAndroid::checkPermission(QString("android.permission.WRITE_EXTERNAL_STORAGE")) == QtAndroid::PermissionResult::Denied){
        QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.WRITE_EXTERNAL_STORAGE"}));
        if (resultHash["android.permission.WRITE_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Denied) {
            return false;
        }
    }

    return true;
}

bool AndroidPlatformTools::getAccessToReadInStorage() {
    if (QtAndroid::checkPermission(QString("android.permission.READ_EXTERNAL_STORAGE")) == QtAndroid::PermissionResult::Denied){
        QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(
                    QStringList({"android.permission.READ_EXTERNAL_STORAGE"}));

        if (resultHash["android.permission.READ_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Denied) {
            return false;
        }
    }

    return true;
}

QString AndroidPlatformTools::storeLink() const {
    return "https://play.google.com/store/apps/details?id=quasarapp.core.cheatcard";
}


}
#endif
