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

    QString cardName = _cards[index.row()];

    auto cacheData = _cache.value(cardName, {});

    if (cacheData.isValid()) {
        return QVariant::fromValue(cacheData.model.data());
    }

    return {};
}

QHash<int, QByteArray> CardsListModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[CardRole] = "card";

    return roles;
}

const QList<QString> &CardsListModel::cards() const {
    return _cards;
}

void CardsListModel::setCards(const QList<QSharedPointer<Card> > &newCards) {
    beginResetModel();

    _cache.clear();

    for (const QSharedPointer<Card>& card : newCards) {
        _cache.insert(card->name(),
                      TableCache
                      {
                          card,
                          QSharedPointer<CardModel>::create(card.data())
                      }
                      );
        _cards.push_back(card->name());
    }

    endResetModel();
}

void CardsListModel::addCard(const QString& name) {
    auto card = QSharedPointer<Card>::create();
    card->setName(name);

    auto cardModel = QSharedPointer<CardModel>::create(card.data());

    beginInsertRows({}, _cards.size(), _cards.size());

    _cache.insert(name, TableCache{card, cardModel});
    _cards.push_back(name);

    endInsertRows();
}

void CardsListModel::removeCard(const QString& cardId) {

    int index = _cards.indexOf(cardId);

    if (index < 0)
        return;

    beginRemoveRows({}, index, index);

    _cache.remove(cardId);
    _cards.removeAt(index);

    endRemoveRows();
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
