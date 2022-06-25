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

// QML
class SettingsKeys: public QObject {
    Q_OBJECT

    Q_PROPERTY(QString CURRENT_USER READ CURRENT_USER CONSTANT)
    Q_PROPERTY(QString CURRENT_LANG READ CURRENT_LANG CONSTANT)
    Q_PROPERTY(QString COLOR_THEME READ COLOR_THEME CONSTANT)
    Q_PROPERTY(QString DARK_THEME READ DARK_THEME CONSTANT)
    Q_PROPERTY(QString SHARE_NAME READ SHARE_NAME CONSTANT)
    Q_PROPERTY(QString CAMERA_DEVICE READ CAMERA_DEVICE CONSTANT)
    Q_PROPERTY(QString DEV_SETTINGS_ENABLE READ DEV_SETTINGS_ENABLE CONSTANT)
    Q_PROPERTY(QString HOST READ HOST CONSTANT)
    Q_PROPERTY(QString FIRST READ FIRST CONSTANT)
    Q_PROPERTY(QString FSELLER READ FSELLER CONSTANT)

    QML_NAMED_ELEMENT(SettingsKeys)

public:
    QString CURRENT_USER();
    QString CURRENT_LANG();
    QString COLOR_THEME();
    QString DARK_THEME();
    QString SHARE_NAME();
    QString CAMERA_DEVICE();
    QString DEV_SETTINGS_ENABLE();
    QString HOST();
    QString FIRST();
    QString FSELLER();


};

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
    bool isBool(const QString& key) const override;
    bool ignoreToRest(const QString& key) const override;
private:

    const QH::ISqlDBCache * _db = nullptr;
};

}

QML_DECLARE_TYPE(RC::SettingsKeys)

#endif // SETTINGSMODEL_H
