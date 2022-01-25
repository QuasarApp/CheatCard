//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef ANDROICBILLING_H
#define ANDROICBILLING_H

#include <QtGlobal>

#ifdef Q_OS_ANDROID

#include <QAndroidJniObject>
#include <QObject>

#include <CheatCardGui/ibilling.h>

class JavaProvider;

class AndroidBilling : public RC::IBilling
{
    Q_OBJECT

public:
    AndroidBilling();

    void init() override;

public slots:
    void becomeSeller() override;
    bool isSupported() const override;

private slots:
    void handlePurchaseReceived(QString id, QString token);

private:
    const char* getPublicKey();
    JavaProvider *_javaProvider = nullptr;

};

#endif
#endif // ANDROICBILLING_H
