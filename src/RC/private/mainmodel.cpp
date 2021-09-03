//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "mainmodel.h"

#include "database.h"
#include "saller.h"

namespace RC {

void noDelete(Saller *) {};

MainModel::MainModel(DB *db) {
    _db = db;
}

bool MainModel::fSeller() const {
    return _sallerModel;
}

void MainModel::makeSaller(Saller *saller) {

    QSharedPointer<Saller> obj{saller, noDelete};
    if (!_db->updateObject(obj, true)) {
        _db->insertObject(obj);
    }

    setSallerModel(saller);
}

QObject *MainModel::sallerModel() const{
    return _sallerModel;
}

void MainModel::setSallerModel(Saller *newSallerModel)
{
    if (_sallerModel == newSallerModel)
        return;
    _sallerModel = newSallerModel;
    emit sallerModelChanged();
}

}
