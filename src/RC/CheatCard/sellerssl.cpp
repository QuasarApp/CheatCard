//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "sellerssl.h"
#include <qaglobalutils.h>

namespace RC {

SellerSSL::SellerSSL(QH::ISqlDBCache *db): Seller(db) {
    useSystemSslConfiguration();

}

NodeType SellerSSL::nodeType() const {
    return Seller::nodeType() | NodeType::SSL;
}
}
