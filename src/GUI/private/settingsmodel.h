//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

#include <CheatCard/database.h>
#include "quasarapp.h"

namespace RC {


class SettingsModel: public QuasarAppUtils::Settings {
    Q_OBJECT
public:
    SettingsModel(const QH::ISqlDBCache* db);
    ~SettingsModel();

    /**
    * @brief init This is simple wrapper of the Settings::init method for convenient access to initialisation.
    * @return instance of the setting.
    */
    static ISettings* init(const QH::ISqlDBCache *db);

    unsigned int getCurrUser();
    void setCurrUser(unsigned int id);

    Q_INVOKABLE void showDataBaseLocation();
    Q_INVOKABLE void exportDataBase();

protected:
    void syncImplementation() override;
    QVariant getValueImplementation(const QString &key, const QVariant &def) override;
    void setValueImplementation(const QString key, const QVariant &value) override;
    QHash<QString, QVariant> defaultSettings() override;

private:

    void forceReloadCache();

    const QH::ISqlDBCache * _db = nullptr;
    unsigned int _currUser = 0;
    QSet<QString> _originalKeys;
    // ISettings interface
};

}

#endif // SETTINGSMODEL_H
