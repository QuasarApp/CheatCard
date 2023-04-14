//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include <QSharedPointer>
#include <getsinglevalue.h>
#include <setsinglevalue.h>
#include "settingsmodel.h"
#include <sqldbwriter.h>
#include <qmlnotifyservice.h>
#include <rcdb/settingskeys.h>

namespace RC {


SettingsModel::SettingsModel() {
}

SettingsModel::~SettingsModel() {

}

QuasarAppUtils::ISettings *SettingsModel::init() {
    initService(std::make_unique<SettingsModel>());
    return instance();
}

QByteArray SettingsModel::getCurrUser() {
    return getValue(P_CURRENT_USER).toByteArray();
}

void SettingsModel::setCurrUser(const QByteArray &id) {
    setValue(P_CURRENT_USER, id);
    forceReloadCache();
}

void SettingsModel::syncImplementation() {
    QuasarAppUtils::Settings::syncImplementation();
}

QVariant SettingsModel::getValueImplementation(const QString &key, const QVariant &def) {

    if (key == P_CURRENT_USER) {
        return QuasarAppUtils::Settings::getValueImplementation(key, def);
    }

    auto currentUser = getCurrUser();
    return QuasarAppUtils::Settings::getValueImplementation(
                QString("%0-%1").arg(currentUser, key), def);

}

void SettingsModel::setValueImplementation(const QString key, const QVariant &value) {
    if (key == P_CURRENT_USER) {
        QuasarAppUtils::Settings::setValueImplementation(key, value);
        return;
    }

    auto currentUser = getCurrUser();
    QuasarAppUtils::Settings::setValueImplementation(
                QString("%0-%1").arg(currentUser, key), value);
}

QHash<QString, QVariant> SettingsModel::defaultSettings() {
    QHash<QString, QVariant> settings;

    settings[P_COLOR_THEME] = "#ff6b01";
    settings[P_DARK_THEME] = false;
    settings[P_SHARE_NAME] = true;
    settings[P_CAMERA_DEVICE] = {};
    settings[P_DEV_SETTINGS_ENABLE] = false;
    settings[P_HOST] = "";
    settings[P_CURRENT_USER] = "";
    settings[P_CURRENT_LANG] = "Auto";


    return settings;
}

bool SettingsModel::isBool(const QString &key) const {

    return key.contains(P_DARK_THEME) ||
           key.contains(P_SHARE_NAME) ||
           key.contains(P_DEV_SETTINGS_ENABLE);
}

bool SettingsModel::ignoreToRest(const QString &key) const {
    return QuasarAppUtils::Settings::ignoreToRest(key);
}

QString SettingsKeys::CURRENT_USER()  {return P_CURRENT_USER; }

QString SettingsKeys::CURRENT_LANG()  {return P_CURRENT_LANG; }

QString SettingsKeys::COLOR_THEME()  {return P_COLOR_THEME; }

QString SettingsKeys::DARK_THEME()  {return P_DARK_THEME; }

QString SettingsKeys::SHARE_NAME()  {return P_SHARE_NAME; }

QString SettingsKeys::CAMERA_DEVICE()  {return P_CAMERA_DEVICE; }

QString SettingsKeys::DEV_SETTINGS_ENABLE()  {return P_DEV_SETTINGS_ENABLE; }

QString SettingsKeys::HOST()  {return P_HOST; }


QString SettingsKeys::FSELLER()  {return P_FSELLER; }

}
