#ifndef NFCNACKEND_H
#define NFCNACKEND_H

#include "CheatCard/iconnectorbackend.h"

class QNearFieldManager;
class QNearFieldTarget;
class QNdefMessage;

namespace RC {
class NFCBackEnd : public RC::IConnectorBackEnd
{
    Q_OBJECT
public:
    NFCBackEnd(DB *_db);

    // IConnectorBackEnd interface
protected:
    bool listen(Mode mode);
    bool close();

private slots:
    void handleConnectionLost(QNearFieldTarget *target);
    void handleConnectionIncomming(QNearFieldTarget *target);
    void targetDetected(const QNdefMessage &message, QNearFieldTarget *target);

private:
    QNearFieldManager *_manager = nullptr;

};

}
#endif // NFCNACKEND_H
