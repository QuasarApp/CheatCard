//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef SELLER_H
#define SELLER_H
#include "iconnectorbackend.h"

namespace RC {

class Seller: public IConnectorBackEnd
{
    Q_OBJECT
public:
    Seller(QH::ISqlDBCache *db);

    // IConnectorBackEnd interface
protected:
    bool hello() override;
};
}
#endif // SELLER_H
