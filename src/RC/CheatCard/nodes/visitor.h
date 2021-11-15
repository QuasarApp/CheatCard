//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef VISITOR_H
#define VISITOR_H

#include "apiv1.h"

namespace RC {


class CHEATCARD_CORE_EXPORT Visitor: public ApiV1
{
    Q_OBJECT
public:
    Visitor(QH::ISqlDBCache *db);
    bool checkCardData(long long session,
                       const QString& domain = DEFAULT_CHEAT_CARD_HOST,
                       int port = DEFAULT_CHEAT_CARD_PORT_SSL);


protected:
    void nodeConnected(QH::AbstractNodeInfo *node) override;

    int getRequestInterval() const;
    void setRequestInterval(int newRequestInterval);

private slots:

    void handleTick();
private:

    int _requestInterval = USERREQUEST_TIMEOUT;

    long long _lastRequested = 0;
    QString _domain = "";
    int _port = 0;

    int _lastRequest = 0;
    QTimer *_timer = nullptr;
};

}
#endif // VISITOR_H
