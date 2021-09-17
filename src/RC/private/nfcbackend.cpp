#include "nfcbackend.h"

#include <QtNfc/qnearfieldmanager.h>
#include "nfcnode.h"
namespace RC {

NFCBackEnd::NFCBackEnd(QH::ISqlDBCache *_db): RC::IConnectorBackEnd(_db) {

    _manager = new QNearFieldManager(this);

    connect(_manager, &QNearFieldManager::targetDetected,
            this, &NFCBackEnd::handleConnectionIncomming);
    connect(_manager, &QNearFieldManager::targetLost,
            this, &NFCBackEnd::handleConnectionLost);
}

bool NFCBackEnd::listen(Mode mode) {
    setMode(mode);
    return _manager->startTargetDetection(QNearFieldTarget::NdefAccess);
}

bool NFCBackEnd::close() {
    _manager->stopTargetDetection();
    return true;
}

void NFCBackEnd::handleConnectionLost(QNearFieldTarget *target) {
    connectionReceived(new NFCNode(target));
}

void NFCBackEnd::handleConnectionIncomming(QNearFieldTarget *target) {
    connectionLost(NFCNode(target).nodeId());
}

}
