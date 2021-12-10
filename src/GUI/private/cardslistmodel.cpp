//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "cardslistmodel.h"
#include "CheatCard/api/api0/card.h"
#include "CheatCard/api/api0/userscards.h"

#include "cardmodel.h"

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

    if (cacheData) {
        return QVariant::fromValue(cacheData.data());
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

void CardsListModel::setCards(const QList<QSharedPointer<API::Card> > &newCards) {
    beginResetModel();

    _cache.clear();

    for (const QSharedPointer<API::Card>& card : newCards) {
        auto cardModel =  QSharedPointer<CardModel>::create(card);
        _cache.insert(card->cardId(),
                      cardModel
                      );
        _cards.push_back(card->cardId());

        configureModel(cardModel);

    }

    endResetModel();
}

QSharedPointer<CardModel>
CardsListModel::importCard(const QSharedPointer<API::Card> &card) {

    if (_cards.contains(card->cardId())) {
        return updateCard(card);
    }

    auto cardModel = QSharedPointer<CardModel>::create(card);

    configureModel(cardModel);

    beginInsertRows({}, _cards.size(), _cards.size());

    _cache.insert(card->cardId(), cardModel);
    _cards.push_back(card->cardId());

    endInsertRows();

    return cardModel;
}

QSharedPointer<CardModel>
CardsListModel::updateCard(const QSharedPointer<API::Card> &card) {

    auto cardModel =  _cache.value(card->cardId());
    if (!cardModel)
        return cardModel;

    cardModel->setCard(card);

    return cardModel;
}

void CardsListModel::updateMetaData(const QList<QSharedPointer<API::UsersCards>> &purchasesNumbers) {
    for (const auto &sp:  purchasesNumbers) {
        if (auto model = _cache.value(sp->getCard())) {
            model->setPurchasesNumber(sp->getPurchasesNumber());
            model->setReceivedItems(sp->getReceived());
        }
    }
}

void CardsListModel::addCard() {
    auto card = QSharedPointer<API::Card>::create();
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

const QHash<int, QSharedPointer<CardModel> > &CardsListModel::cache() const {
    return _cache;
}

void CardsListModel::configureModel(const QSharedPointer<CardModel> &cardModel) {
    connect(cardModel.data(), &CardModel::editFinished, this, &CardsListModel::sigEditFinished);
    connect(cardModel.data(), &CardModel::sigRemoveRequest, this, &CardsListModel::sigRemoveRequest);
    connect(cardModel.data(), &CardModel::sigActivate, this, &CardsListModel::handleActivate);
    connect(cardModel.data(), &CardModel::sigShowStatistick, this, &CardsListModel::handleShowStatistic);

}


void CardsListModel::handleActivate(const QSharedPointer<API::Card>& card) {
    auto cardModel = _cache.value(card->cardId());

    if (cardModel) {
        emit sigCardSelectedForWork(cardModel);
    }
}

void CardsListModel::handleShowStatistic(const QSharedPointer<API::Card>& card) {
    auto cardModel = _cache.value(card->cardId());

    if (cardModel) {
        emit sigCardSelectedForStatistic(cardModel);
    }
}
}
