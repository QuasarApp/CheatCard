//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

#include "quasarapp.h"

namespace RC {


class SettingsModel: public QuasarAppUtils::ISettings {
public:
    SettingsModel();

    // ISettings interface
protected:
    void syncImplementation();
    QVariant getValueImplementation(const QString &key, const QVariant &def);
    void setValueImplementation(const QString key, const QVariant &value);
};

}

#endif // SETTINGSMODEL_H
