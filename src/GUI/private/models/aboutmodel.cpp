//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "aboutmodel.h"
#include "heart.h"
#include "CheatCard/basenode.h"
#include <doctorpill.h>
#include <qmlnotifyservice.h>

namespace RC {

AboutModel::AboutModel() {

}

QString AboutModel::getCoreLibVr() const {
    return RC::BaseNode::libVersion();
}

QString AboutModel::getHeartLibVr() const {
    return QH::heartLibVersion();
}

QString AboutModel::getZxingCppLibVr() const {
    return "";
}

QString AboutModel::getSCodesLibVr() const {
    return "";
}

QString AboutModel::getQtLibVr() const {
    return QT_VERSION_STR;
}

QString AboutModel::getSimpleQmlNotifyLibVr() const {
    return QmlNotificationService::NotificationService::libVersion();
}

QString AboutModel::getDoctorPillLibVr() const {
    return DP::version();
}

}
