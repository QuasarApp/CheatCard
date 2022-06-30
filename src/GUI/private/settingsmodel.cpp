//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include <QSharedPointer>
#include <getsinglevalue.h>
#include <setsinglevalue.h>
#include "settingsmodel.h"
#include "dbaddress.h"
#include "iplatformtools.h"
#include <sqldbwriter.h>
#include <qmlnotifyservice.h>
#include <CheatCard/settingskeys.h>

namespace RC {


SettingsModel::SettingsModel(const QH::ISqlDBCache *db) {
    _db = db;
}

SettingsModel::~SettingsModel() {

}

QuasarAppUtils::ISettings *SettingsModel::init(const QH::ISqlDBCache *db) {
    return QuasarAppUtils::ISettings::init<SettingsModel>(db);
}

unsigned int SettingsModel::getCurrUser() {
    return getValue(P_CURRENT_USER).toInt();
}

void SettingsModel::setCurrUser(unsigned int id) {
    setValue(P_CURRENT_USER, id);
    forceReloadCache();
}

void SettingsModel::showDataBaseLocation() {
    QString db;
    if (_db && _db->writer()) {
        db = _db->writer()->databaseLocation();
    }

    if (auto service = QmlNotificationService::NotificationService::getService()) {
        service->setNotify(tr("Database location"),
                           db,
                           "", QmlNotificationService::NotificationData::Normal);

        return;
    }
}

void SettingsModel::exportDataBase() {

    auto platformsTools = IPlatformTools::instance();

    if (!(platformsTools && platformsTools->getAccessToWriteInStorage())) {
        return;
    }

    QString exportDbPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString date = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm");
    QString exportDb = exportDbPath + "/CheatCard." + date + ".db";
    QString db;
    if (_db && _db->writer()) {
        db = _db->writer()->databaseLocation();
    }

    if (QFileInfo::exists(exportDb)) {
        QFile::remove(exportDb);
    } else {
        QDir().mkpath(exportDbPath);
    }

    auto service = QmlNotificationService::NotificationService::getService();

    if (!QFile::copy(db, exportDb)) {
        if (service) {
            service->setNotify(tr("Failed to export database"),
                               tr("From %0 to %1").arg(db, exportDb),
                               "", QmlNotificationService::NotificationData::Error);

            return;
        }
    }

    if (service) {
        service->setNotify(tr("Export the database finished successful"),
                           tr("From %0 to %1").arg(db, exportDb),
                           "", QmlNotificationService::NotificationData::Normal);
    }
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
                QString("%0-%1").arg(currentUser).arg(key), def);

}

void SettingsModel::setValueImplementation(const QString key, const QVariant &value) {
    if (key == P_CURRENT_USER) {
        QuasarAppUtils::Settings::setValueImplementation(key, value);
        return;
    }

    auto currentUser = getCurrUser();
    QuasarAppUtils::Settings::setValueImplementation(
                QString("%0-%1").arg(currentUser).arg(key), value);
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
