#ifndef SETTINGSKEYS_H
#define SETTINGSKEYS_H

#include <QObject>
#include <QtQml/qqml.h>


// cpp
#define P_CURRENT_USER "CURRENT_USER"
#define P_CURRENT_LANG "CURRENT_LANG"
#define P_COLOR_THEME "colorTheme"
#define P_DARK_THEME "darkTheme"
#define P_SHARE_NAME "shareName"
#define P_CAMERA_DEVICE "cameraDevice"
#define P_DEV_SETTINGS_ENABLE "devSettingEnable"
#define P_HOST "host"
#define P_API_VERSION "APIVersion"
#define P_FIRST "fFirst"

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
    Q_PROPERTY(QString API_VERSION READ API_VERSION CONSTANT)
    Q_PROPERTY(QString FIRST READ FIRST CONSTANT)

    QML_ELEMENT

public:
    static inline QString CURRENT_USER()  {return P_CURRENT_USER; };
    static inline QString CURRENT_LANG()  {return P_CURRENT_LANG; };
    static inline QString COLOR_THEME()  {return P_COLOR_THEME; };
    static inline QString DARK_THEME()  {return P_DARK_THEME; };
    static inline QString SHARE_NAME()  {return P_SHARE_NAME; };
    static inline QString CAMERA_DEVICE()  {return P_CAMERA_DEVICE; };
    static inline QString DEV_SETTINGS_ENABLE()  {return P_DEV_SETTINGS_ENABLE; };
    static inline QString HOST()  {return P_HOST; };
    static inline QString API_VERSION()  {return P_API_VERSION; };
    static inline QString FIRST()  {return P_FIRST; };


};

#endif // SETTINGSKEYS_H
