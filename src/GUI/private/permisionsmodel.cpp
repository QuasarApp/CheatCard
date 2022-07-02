//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "permisionsmodel.h"

namespace RC {

PermisionsModel::PermisionsModel() {
}

void PermisionsModel::setPermissions(const QList<QSharedPointer<RC::API::Contacts> > &newData) {
    beginResetModel();
    _data.clear();
    _permissions.clear();
    for (const QSharedPointer<API::Contacts>& permision : newData) {
        _data.insert(permision->getGenesisKey(),
                      permision
                      );
        _permissions.push_back(permision->getGenesisKey());
    }
    endResetModel();
}


int PermisionsModel::rowCount(const QModelIndex &) const {
    return _data.size();
}

QHash<int, QByteArray> PermisionsModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[PermisionRole] = "permissionName";

    return roles;
}

QVariant PermisionsModel::data(const QModelIndex &index, int role) const {

    if (role != PermisionRole) {
        return {};
    }

    if (index.row() >= rowCount()) {
        return {};
    }

    int permisionKey = _permissions[index.row()];
    auto cacheData = _data.value(permisionKey, {});

    if (cacheData) {
        return cacheData->getInfo();
    }

    return {};
}

void PermisionsModel::setNewDescription(int row, const QString &description) {
    if (row >= rowCount()) {
        return;
    }

    int permisionKey = _permissions[row];

    if (_data.contains(permisionKey)) {
        _data[permisionKey]->setInfo(description);

        emit dataChanged(index(row), index(row), {PermisionRole});
        emit sigPermision(_data[permisionKey]);
    }


}
}
