//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "mainmodel.h"

#include "database.h"

namespace RC {

MainModel::MainModel(DB *db) {
    _db = db;
}

bool MainModel::getFSeller() const {
    return fSeller;
}

void MainModel::makeSaller(Saller *saller) {

}

void MainModel::setFSeller(bool newFSeller) {
    if (fSeller == newFSeller)
        return;
    fSeller = newFSeller;
    emit fSellerChanged();
}

}
