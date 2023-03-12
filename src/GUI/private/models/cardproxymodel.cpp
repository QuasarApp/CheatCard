//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#
#include "cardmodel.h"
#include "cardproxymodel.h"
#include "rci/objects/icard.h"
namespace RC {

CardProxyModel::CardProxyModel() {

}

void CardProxyModel::removeCard(const QByteArray& cardId) {
    if (auto source = dynamic_cast<iCardListModel*>(sourceModel())) {
        source->removeCard(cardId);
    }
}

void CardProxyModel::activateCard(const QByteArray &cardId) {
    if (auto source = dynamic_cast<iCardListModel*>(sourceModel())) {
        source->activateCard(cardId);
    }
}

void CardProxyModel::activateCardByIndex(int index) {
    if (auto source = dynamic_cast<iCardListModel*>(sourceModel())) {
        source->activateCardByIndex(mapToSource(this->index(index, 0)).row());
    }
}

bool CardProxyModel::lessThan(const QModelIndex &source_left,
                              const QModelIndex &source_right) const {

    QObject * obj = qvariant_cast<QObject *>(source_left.data(Qt::UserRole));
    CardModel * left = qobject_cast<CardModel*>(obj);

    obj = qvariant_cast<QObject *>(source_right.data(Qt::UserRole));
    CardModel * right = qobject_cast<CardModel*>(obj);

    if (!(right && left))
        return false;

    if (left->card()->title().isEmpty())
        return false;

    return left->card()->title() < right->card()->title();
}

}
