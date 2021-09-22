//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "nfcbackend.h"

#include <QtNfc/qnearfieldmanager.h>
#include "nfcnode.h"
#include <qmlnotifyservice.h>

namespace RC {

NFCBackEnd::NFCBackEnd(QH::ISqlDBCache *_db): RC::IConnectorBackEnd(_db) {

    _manager = new QNearFieldManager(this);


#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
    return _manager->startTargetDetection(QNearFieldTarget::NdefAccess);
#else
    _manager->setTargetAccessModes(QNearFieldManager::NdefReadTargetAccess |
                                   QNearFieldManager::NdefWriteTargetAccess);
    int id = _manager->registerNdefMessageHandler(this, SLOT(targetDetected(QNdefMessage,QNearFieldTarget*)));

    if (id < 0) {
        QuasarAppUtils::Params::log("Failed to register nfc handler",
                                    QuasarAppUtils::Error);


        auto service = QmlNotificationService::NotificationService::getService();
        service->setNotify("NFC Error", "Failed to register nfc handler");
    }
#endif


    connect(_manager, &QNearFieldManager::targetDetected,
            this, &NFCBackEnd::handleConnectionIncomming);
    connect(_manager, &QNearFieldManager::targetLost,
            this, &NFCBackEnd::handleConnectionLost);
}

bool NFCBackEnd::listen(Mode mode) {
    setMode(mode);

    return _manager->startTargetDetection();
}

bool NFCBackEnd::close() {
    _manager->stopTargetDetection();
    return true;
}

void NFCBackEnd::handleConnectionLost(QNearFieldTarget *target) {
    connectionLost(NFCNode(target).nodeId());

}

void NFCBackEnd::handleConnectionIncomming(QNearFieldTarget *target) {
    connectionReceived(new NFCNode(target));   
}

void NFCBackEnd::targetDetected(const QNdefMessage &message, QNearFieldTarget *target) {
    auto node = new NFCNode(target);

    connectionReceived(node);

    node->handleReceiveRawData(message);

}

}
