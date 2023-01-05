//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef VISITOR_H
#define VISITOR_H

#include "basenode.h"

namespace RC {


class CHEATCARD_CORE_EXPORT Visitor: public BaseNode
{
    Q_OBJECT
public:
    Visitor(const QSharedPointer<Interfaces::iDB> &db);
    bool checkCardData(long long session,
                       const QString& domain = "",
                       int port = DEFAULT_CHEAT_CARD_PORT_SSL);

    NodeType nodeType() const override;

protected:
    void nodeConnected(QH::AbstractNodeInfo *node) override;
    void nodeConfirmend(QH::AbstractNodeInfo *node) override;

    int getRequestInterval() const;
    void setRequestInterval(int newRequestInterval);


private slots:

    void handleTick();
private:
    bool sendRequestPrivate();

    int _requestInterval = USERREQUEST_TIMEOUT;
    QString _domain;
    int _port;
    long long _lastRequested = 0;

    int _lastRequest = 0;
    QTimer *_timer = nullptr;
};

}
#endif // VISITOR_H
