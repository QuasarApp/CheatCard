#include "nfcbackend.h"

#include <QtNfc/qnearfieldmanager.h>
#include "nfcnode.h"
namespace RC {

NFCBackEnd::NFCBackEnd(QH::ISqlDBCache *_db): RC::IConnectorBackEnd(_db) {

    _manager = new QNearFieldManager(this);

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
#else
    _manager->registerNdefMessageHandler(this, SLOT(targetDetected(QNdefMessage,QNearFieldTarget*)));
#endif

    connect(_manager, &QNearFieldManager::targetDetected,
            this, &NFCBackEnd::handleConnectionIncomming);
    connect(_manager, &QNearFieldManager::targetLost,
            this, &NFCBackEnd::handleConnectionLost);
}

bool NFCBackEnd::listen(Mode mode) {
    setMode(mode);
#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
    return _manager->startTargetDetection(QNearFieldTarget::NdefAccess);
#else
    _manager->setTargetAccessModes(QNearFieldManager::NdefReadTargetAccess |
                                   QNearFieldManager::NdefWriteTargetAccess);
    return _manager->startTargetDetection();
#endif
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
