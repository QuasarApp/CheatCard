//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef NODETYPE_H
#define NODETYPE_H

#include <quasarapp.h>

namespace RC {
enum class NodeType: int {
    // Base types (first 4 bits)
    Unknown = 0x00,
    Visitor = 0x01,
    Seller  = 0x02,
    Server  = 0x03,

    BaseTypeMask = 0x0f,
// ===========
// Encript Type
    SSL     = 0x10,

    CryptTypeMask = 0xf0,
};

class NodeTypeHelper {
public:
    static NodeType getBaseType(NodeType type);
    static NodeType getSSLType(NodeType type);

};
}

#endif // NODETYPE_H
