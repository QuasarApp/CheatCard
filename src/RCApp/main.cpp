//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include <QGuiApplication>
#include <quasarapp.h>
#include "CheatCardGui/CheatCard.h"
#include <QSslSocket>
#include <novalidationbilling.h>
#include <qmlnotifyservice.h>
#include "androidbilling.h"
#include "desktopbilling.h"
#include "CheatCard/settingskeys.h"

void initLang() {
    QLocale locale = QLocale::system();
    QSettings setting;
    QString userLang = setting.value(CURRENT_LANG).toString();

    if (!userLang.isEmpty()) {
        locale = QLocale(userLang);
    }

    QString customLanguage = QuasarAppUtils::Params::getArg("lang");
    if(customLanguage.size()) {
        locale = QLocale(customLanguage);
    }

    if(!QuasarAppUtils::Locales::init(locale, {":/CheatCardTr/languages/",
                                      ":/credits_languages/",
                                      ":/qmlNotify_languages/",
                                      ":/lv_languages/",
                                      ":/DoctorPillTr/languages"})){
        QuasarAppUtils::Params::log("Error load language : ", QuasarAppUtils::Error);
    }
}

RC::IBilling * getBillingInstance() {
#ifdef ALLOW_BILLING_APP
    return new NoValidationBilling;
#endif

#ifdef Q_OS_ANDROID
#ifdef USE_SIGN_APP
    return new AndroidBilling;
#endif
#endif

#ifdef Q_OS_LINUX
    return new DesktopBilling;
#endif

    return nullptr;
}

int main(int argc, char *argv[]) {
#ifdef Q_OS_ANDROID
    qputenv("ANDROID_OPENSSL_SUFFIX", "_1_1");
#endif
    QuasarAppUtils::Params::parseParams(argc, argv);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // <--
#endif
    QCoreApplication::setOrganizationName("QuasarApp");
    QCoreApplication::setApplicationName("CheatCard");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;


    if (!QmlNotificationService::init(&engine)) {
        return 0;
    }

    RC::CheatCard rc;
    initLang();

    if (!rc.init(&engine, getBillingInstance())) {
        return 0;
    }
    return app.exec();
}
