//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "cardslistmodel.h"
#include "card.h"
#include "cardmodel.h"

namespace RC {

CardsListModel::CardsListModel(DB *db) {
    _db = db;
}

int CardsListModel::rowCount(const QModelIndex &) const {
    return _cards.size();
}

QVariant CardsListModel::data(const QModelIndex &index, int role) const {

    if (role != CardRole) {
        return {};
    }

    if (index.row() >= rowCount()) {
        return {};
    }

    int cardId = _cards[index.row()];

    auto cacheData = _cache.value(cardId, {});

    if (cacheData.isValid()) {
        return QVariant::fromValue(cacheData.model);
    }

    return {};
}

QHash<int, QByteArray> CardsListModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[CardRole] = "card";

    return roles;
}

const QList<int> &CardsListModel::cards() const {
    return _cards;
}

void CardsListModel::setCards(const QHash<int, QSharedPointer<Card> > &newCards) {
    beginResetModel();

    _cache.clear();

    for (const QSharedPointer<Card>& card : newCards) {
        _cache.insert(card->getId().toInt(), TableCache{card, QSharedPointer<CardModel>::create(card.data())});
    }

    _cards = QList(newCards.keyBegin(), newCards.keyEnd());

    endResetModel();
}

TableCache::TableCache() {

}

TableCache::TableCache(const QSharedPointer<Card> &src,
                       const QSharedPointer<CardModel> &mod) {
    source = src;
    model = mod;
}

bool TableCache::isValid() const {
    return !source.isNull();
}


}
