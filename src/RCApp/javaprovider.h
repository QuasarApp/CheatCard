//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef JAVAPROVIDER_H
#define JAVAPROVIDER_H

#include <QtGlobal>

#ifdef Q_OS_ANDROID

#include <QAndroidJniObject>
#include <QObject>

struct Purchase {
    QString id;
    QString token;
};

class JavaProvider : public QObject
{
    Q_OBJECT

public:
    static JavaProvider* instance();

signals:
    void sigPurchase(Purchase);

private:
    JavaProvider();
    void getPremium() const;

    static void purchaseReceived(JNIEnv *env, jobject thiz, jstring id, jstring token);
};

#endif
#endif // JAVAPROVIDER_H
