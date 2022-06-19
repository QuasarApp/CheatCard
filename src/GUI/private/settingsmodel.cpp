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


SettingsModel::SettingsModel(QH::ISqlDBCache *db) {
    _db = db;
}

SettingsModel::~SettingsModel() {

}

unsigned int SettingsModel::getCurrUser() {
    return _currUser;
}

void SettingsModel::setCurrUser(unsigned int id) {
    _currUser = id;

    QH::PKG::GetSingleValue req({"Config", id}, "user", "user");
    auto result = _db->getObject(req);

    if (!result || result->value().toUInt() != id) {
        _db->doQuery(QString("INSERT INTO Config(user) VALUES(%0)").arg(id));
    }

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

}

QVariant SettingsModel::getValueImplementation(const QString &key, const QVariant &def) {

    QH::PKG::GetSingleValue request({"Config", _currUser}, key, "user");
    auto result = _db->getObject(request);

    if (!result || result->value().isNull()) {
        return def;
    }

    return result->value();

}

void SettingsModel::setValueImplementation(const QString key, const QVariant &value) {

    auto updateRequest = QSharedPointer<QH::PKG::SetSingleValue>::create(
                QH::DbAddress{"Config", _currUser}, key, value, "user");

    _db->updateObject(updateRequest);

}


}
