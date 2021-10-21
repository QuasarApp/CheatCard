#include "desktopbilling.h"
#include "CheatCard/basenode.h"

#define TEST_SELLER_ID "TestSeller"

DesktopBilling::DesktopBilling() {

}

void DesktopBilling::init() {
    auto token = _settings.value(TEST_SELLER_ID, "").toString();

    if(!token.isEmpty()) {
        sendPurchase(token);
    }
}

void DesktopBilling::sendPurchase(const QString& token) {
    RC::Purchase purchase;
    purchase.id = "test";
    purchase.token = TEST_SELLER_TOKEN;
    emit sigPurchaseReceived(purchase);
}

void DesktopBilling::becomeSeller() {
    _settings.setValue(TEST_SELLER_ID, TEST_SELLER_TOKEN);
    sendPurchase(TEST_SELLER_TOKEN);

}
