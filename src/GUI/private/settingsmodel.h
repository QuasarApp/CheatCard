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


class SettingsModel: public QuasarAppUtils::ISettings {

public:
    SettingsModel(QH::ISqlDBCache* db);
    ~SettingsModel();

    unsigned int getCurrUser();
    void setCurrUser(unsigned int id);

signals:
    void colorThemeChanged();

protected:
    void syncImplementation();
    QVariant getValueImplementation(const QString &key, const QVariant &def);
    void setValueImplementation(const QString key, const QVariant &value);

private:
    QH::ISqlDBCache * _db = nullptr;
    unsigned int _currUser = 0;
};

}

#endif // SETTINGSMODEL_H
