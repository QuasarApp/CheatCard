//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of

#include "config.h"

namespace RC {

Config::Config() : QH::PKG::DBObject("Config"){

}

QH::PKG::DBObject *Config::createDBObject() const {
    return new Config;
}

bool Config::fromSqlRecord(const QSqlRecord &q) {
    userId = q.value("user").toInt();
    firstRun = q.value("fFirst").toBool();

    return true;
}

QH::PKG::DBVariantMap Config::variantMap() const {
    return {{"user",      {userId,        QH::PKG::MemberType::PrimaryKey}},
            {"fFirst",    {firstRun,      QH::PKG::MemberType::InsertUpdate}},
    };
}

bool Config::isValid() const {
    return userId >= 0;
}

QString Config::primaryKey() const {
    return "";
}

bool Config::getFirstRun() const {
    return firstRun;
}

void Config::setFirstRun(bool newFirstRun) {
    firstRun = newFirstRun;
}

int Config::getUserId() const {
    return userId;
}

void Config::setUserId(int newUserId) {
    userId = newUserId;
}
}
