//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "nodetype.h"
#include <qaglobalutils.h>

namespace RC {

NodeType NodeTypeHelper::getBaseType(NodeType type) {
    return type & NodeType::BaseTypeMask;
}

NodeType NodeTypeHelper::getSSLType(NodeType type) {
    return type & NodeType::CryptTypeMask;
}
}
