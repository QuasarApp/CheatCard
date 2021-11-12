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

namespace RC {


SettingsModel::SettingsModel(QH::ISqlDBCache *db) {
    _db = db;
}

SettingsModel::~SettingsModel() {

}

void SettingsModel::syncImplementation() {

}

QVariant SettingsModel::getValueImplementation(const QString &key, const QVariant &def) {

    QH::PKG::GetSingleValue request({"Config", "There will be id"}, key);
    auto result = _db->getObject(request);

    if (!result) {
        return def;
    }

    return result->value();

}

void SettingsModel::setValueImplementation(const QString key, const QVariant &value) {

    auto updateRequest = QSharedPointer<QH::PKG::SetSingleValue>::create(
                QH::DbAddress{"Config", "There will be id"}, key, value);

    _db->updateObject(updateRequest);

}

}
