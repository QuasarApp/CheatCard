//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef VISITOR_H
#define VISITOR_H

#include "client.h"

namespace RC {


class CHEATCARD_CORE_EXPORT Visitor: public Client
{
    Q_OBJECT
public:
    Visitor(const QSharedPointer<Interfaces::iDB> &db);
    NodeType nodeType() const override;

protected:
    void nodeConnected(QH::AbstractNodeInfo *node) override;
    void nodeConfirmend(QH::AbstractNodeInfo *node) override;
};

}
#endif // VISITOR_H
