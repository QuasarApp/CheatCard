//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "seller.h"
namespace RC {

Seller::Seller(DB *db): IConnectorBackEnd(db) {
    setMode(Saller);
}

bool Seller::hello() {

}
}
