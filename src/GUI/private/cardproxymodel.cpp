//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#
#include "cardmodel.h"
#include "cardproxymodel.h"
#include "cardslistmodel.h"
namespace RC {

CardProxyModel::CardProxyModel()
{

}

void CardProxyModel::addCard() {
    if (auto source = dynamic_cast<iCardListModel*>(sourceModel())) {
        source->addCard();
    }
}

void CardProxyModel::removeCard(int cardId) {
    if (auto source = dynamic_cast<iCardListModel*>(sourceModel())) {
        source->removeCard(cardId);
    }
}

void CardProxyModel::cardSelected(int cardId, int type) {
    if (auto source = dynamic_cast<iCardListModel*>(sourceModel())) {
        source->cardSelected(cardId, type);
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
