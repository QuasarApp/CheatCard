//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "visitor.h"
#include <cstring>
#include "api/apibase.h"


namespace RC {


Visitor::Visitor(const QSharedPointer<Interfaces::iDB> &db): Client(db) {
}

BaseNode::NodeType Visitor::nodeType() const {
    return static_cast<BaseNode::NodeType>(API::APIBase::NodeType::Visitor);
}

void Visitor::nodeConnected(QH::AbstractNodeInfo *node) {
    Client::nodeConnected(node);
}

void Visitor::nodeConfirmend(QH::AbstractNodeInfo *node) {
    Client::nodeConfirmend(node);
}
}
