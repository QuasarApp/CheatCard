#ifndef DESKTOPBILLING_H
#define DESKTOPBILLING_H

#include <CheatCardGui/ibilling.h>
#include "QSettings"


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

private:
    QSettings _settings;
    void sendPurchase(const QString &token);
};

#endif // DESKTOPBILLING_H
