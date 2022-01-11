//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "pillsmodel.h"
#include <qmlnotifyservice.h>
#include <notificationdata.h>
#include <QHash>

namespace RC {

PillsModel::PillsModel()
{

}

int PillsModel::rowCount(const QModelIndex &) const {
    return _viewData;
}

QVariant PillsModel::data(const QModelIndex &index, int role) const {

    if (index.row() < 0 || index.row() >= rowCount()) {
        return "";
    }

    const auto &item = _doctor.pillsData().at(index.row());

    if (role == Roles::Name) {
        return item->name();
    }

    if (role == Roles::Description) {
        return item->description();
    }

    return "Unknown";
}

QHash<int, QByteArray> PillsModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[Roles::Name] = "pillName";
    roles[Roles::Description] = "pillDescription";
    return roles;
}

void PillsModel::usePill(int index) {

    if (index < 0 || index >= rowCount()) {
        return;
    }

    auto &pill = _doctor.pillsData().at(index);
    _doctor.fix({pill});
}

void PillsModel::handleFixFailed(QList<QSharedPointer<QuasarAppUtils::iPill>> failed) const {

    QStringList fails;
    for (const auto &pill : qAsConst(failed)) {
        fails.push_back(pill->name());
    }

    auto service = QmlNotificationService::NotificationService::getService();
    if (service) {
        service->setNotify(tr("Failed to fix bugs!"),
                           tr("List of incoplite bugs: %0").arg(fails.join(',')));
    }
}

void PillsModel::handleFixSuccessful(QList<QSharedPointer<QuasarAppUtils::iPill>> successful) const {

    QStringList successfull;
    for (const auto &pill : qAsConst(successful)) {
        successfull.push_back(pill->name());
    }

    auto service = QmlNotificationService::NotificationService::getService();
    if (service) {
        service->setNotify(tr("Bug fixed successfull!"),
                           tr("List of fixed bugs: %0").arg(successfull.join(',')));
    }
}

void PillsModel::handleBugDetected(QList<QSharedPointer<QuasarAppUtils::iPill>> bugDetected) {

    beginResetModel();

    _viewData.clear();
    for (const auto &pill : qAsConst(bugDetected)) {
        _viewData[pill->name()] = pill;
    }

    endResetModel();
}
}
