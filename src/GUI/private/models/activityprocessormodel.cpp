//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "activityprocessormodel.h"

#include <QFile>
namespace RC {

ActivityProcessorModel::ActivityProcessorModel() {

}

bool ActivityProcessorModel::newActivity(const QString &qmlView, QObject *model) {
    emit sigNewActivity(qmlView, model);
    return true;
}

bool ActivityProcessorModel::freeItem(QObject *card, const QByteArray& userId, int freeItemsCount) {
    if (!(card && userId.size()))
        return false;

    emit sigFreeItem(card, userId, freeItemsCount);
    return true;
}

}
