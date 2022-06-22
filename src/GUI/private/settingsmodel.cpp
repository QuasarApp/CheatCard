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
    return _currUser;
}

void SettingsModel::setCurrUser(unsigned int id) {
    _currUser = id;
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
    return QuasarAppUtils::Settings::getValueImplementation(
                QString("%0-%1").arg(_currUser).arg(key), def);

}

void SettingsModel::setValueImplementation(const QString key, const QVariant &value) {
    QuasarAppUtils::Settings::setValueImplementation(
                QString("%0-%1").arg(_currUser).arg(key), value);
    _originalKeys += key;
}

QHash<QString, QVariant> SettingsModel::defaultSettings() {
    QHash<QString, QVariant> settings;

    settings["colorTheme"] = "#ff6b01";
    settings["shareName"] = true;
    settings["cameraDevice"] = {};
    settings["devSettingEnable"] = false;
    settings["host"] = "";
    settings["APIVersion"] = 2;

    return settings;
}

void SettingsModel::forceReloadCache() {
    for (auto it = _originalKeys.begin(); it != _originalKeys.end(); ++it) {
        auto val = getValueImplementation(*it, {});
        if (!val.isNull())
            setValue(*it, val);
    }
}

}
