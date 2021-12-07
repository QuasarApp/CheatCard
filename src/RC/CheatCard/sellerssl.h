//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SELLERSSL_H
#define SELLERSSL_H

#include "seller.h"

namespace RC {

namespace API {
class UserHeader;
}

class CHEATCARD_CORE_EXPORT SellerSSL: public Seller
{
    Q_OBJECT
public:
    SellerSSL(QH::ISqlDBCache *db);

};
}
#endif // SELLERSSL_H
