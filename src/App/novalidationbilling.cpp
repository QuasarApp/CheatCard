#include "novalidationbilling.h"

NoValidationBilling::NoValidationBilling()
{

}

void NoValidationBilling::becomeSeller() {
    _settings.setValue(TEST_SELLER_ID, TEST_SELLER_TOKEN);
    sendPurchase(TEST_SELLER_TOKEN);
}
