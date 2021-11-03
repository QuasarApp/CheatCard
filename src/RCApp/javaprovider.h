//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef JAVAPROVIDER_H
#define JAVAPROVIDER_H

#include <QtGlobal>

#ifdef Q_OS_ANDROID

#include <QAndroidJniObject>
#include <QObject>

class JavaProvider : public QObject
{
    Q_OBJECT

public:
    static JavaProvider* instance();
    void getPremium() const;
    void initBilling() const;

signals:
    void sigPurchase(QString id, QString token);

private:
    JavaProvider();

    static void purchaseReceived(JNIEnv *env, jobject thiz, jstring id, jstring token);
};

#endif
#endif // JAVAPROVIDER_H
