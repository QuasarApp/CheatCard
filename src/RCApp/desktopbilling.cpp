#include "desktopbilling.h"
#include "CheatCard/basenode.h"

#include <qmlnotifyservice.h>

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
    purchase.token = token;
    emit sigPurchaseReceived(purchase);
}

void DesktopBilling::becomeSeller() {

    auto service = QmlNotificationService::NotificationService::getService();

    QmlNotificationService::Listner listner = [this] (bool accepted) {

        if (accepted) {
            _settings.setValue(TEST_SELLER_ID, TEST_SELLER_TOKEN);
            sendPurchase(TEST_SELLER_TOKEN);
        }

    };

    service->setQuestion(listner, tr("Become a seller"),
                         tr("This functions unlocked only on test mode."
                            " Do you realy want to become a seller?"));
}

bool DesktopBilling::isSupported() {
    return true;
}
