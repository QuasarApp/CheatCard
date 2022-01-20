//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "visitorssl.h"
#include <qaglobalutils.h>

namespace RC {

VisitorSSL::VisitorSSL(QH::ISqlDBCache *db): Visitor(db) {
    useSystemSslConfiguration();
}

NodeType VisitorSSL::nodeType() const {
    return Visitor::nodeType() | NodeType::SSL;

}
}
