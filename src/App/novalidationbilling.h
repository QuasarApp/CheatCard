#ifndef NOVALIDATIONBILLING_H
#define NOVALIDATIONBILLING_H

#include "desktopbilling.h"



class NoValidationBilling: public DesktopBilling
{
    Q_OBJECT
public:
    NoValidationBilling();

public slots:
    void becomeSeller() override;
};

#endif // NOVALIDATIONBILLING_H
