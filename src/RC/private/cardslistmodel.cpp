//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "cardslistmodel.h"
#include "card.h"
#include "cardmodel.h"
#include "userscards.h"

namespace RC {

CardsListModel::CardsListModel() {
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
        return QVariant::fromValue(cacheData.model.data());
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

void CardsListModel::setCards(const QList<QSharedPointer<Card> > &newCards) {
    beginResetModel();

    _cache.clear();

    for (const QSharedPointer<Card>& card : newCards) {
        auto cardModel =  QSharedPointer<CardModel>::create(card);
        _cache.insert(card->cardId(),
                      TableCache
                      {
                          card,
                          cardModel
                      }
                      );
        _cards.push_back(card->cardId());

        configureModel(cardModel);

    }

    endResetModel();
}

QSharedPointer<CardModel>
CardsListModel::importCard(const QSharedPointer<Card> &card) {

    auto cardModel = QSharedPointer<CardModel>::create(card);

    configureModel(cardModel);

    beginInsertRows({}, _cards.size(), _cards.size());

    _cache.insert(card->cardId(), TableCache{card, cardModel});
    _cards.push_back(card->cardId());

    endInsertRows();



    return cardModel;
}

void CardsListModel::setPurchasesNumbers(const QList<QSharedPointer<UsersCards>> &purchasesNumbers) {
    for (const auto &sp:  purchasesNumbers) {
        if (auto model = _cache.value(sp->getCard()).model) {
            model->setPurchasesNumber(sp->getPurchasesNumber());
        }
    }
}

void CardsListModel::addCard() {
    auto card = QSharedPointer<Card>::create();
    importCard(card);
}

void CardsListModel::removeCard(int cardId) {

    int index = _cards.indexOf(cardId);

    if (index < 0)
        return;

    beginRemoveRows({}, index, index);

    _cache.remove(cardId);
    _cards.removeAt(index);

    endRemoveRows();

    emit sigCardRemoved(cardId);
}

void CardsListModel::cardSelected(int cardId) {
    emit sigCardSelectedForWork(_cache.value(cardId).model);
}

const QHash<int, TableCache> &CardsListModel::cache() const {
    return _cache;
}

void CardsListModel::configureModel(const QSharedPointer<CardModel> &cardModel) {
    connect(cardModel.data(), &CardModel::editFinished, this, &CardsListModel::sigEditFinished);
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
