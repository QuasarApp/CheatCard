#ifndef DESKTOPBILLING_H
#define DESKTOPBILLING_H

#include <CheatCardGui/ibilling.h>
#include "QSettings"

#define TEST_SELLER_ID "TestSeller"
#define TEST_SELLER_TOKEN "testbase64tokenofseller"

class DesktopBilling: public RC::IBilling
{
    Q_OBJECT
public:
    DesktopBilling();

    // IBilling interface
public:
    void init() override;

public slots:
    void becomeSeller() override;
    bool isSupported() override;

protected:
    void sendPurchase(const QString &token);
    QSettings _settings;
};

#endif // DESKTOPBILLING_H
