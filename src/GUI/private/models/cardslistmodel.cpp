//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "cardslistmodel.h"

#include "cardmodel.h"
#include "rci/objects/icard.h"
#include "rci/objects/iuserscards.h"

namespace RC {

CardsListModel::CardsListModel() {
}

int CardsListModel::rowCount(const QModelIndex &) const {
    return _cards.size();
}

QVariant CardsListModel::data(const QModelIndex &index, int role) const {


    if (index.row() >= rowCount()) {
        return {};
    }

    const QByteArray& cardId = _cards[index.row()];
    auto cacheData = _cache.value(cardId, {});

    if (cacheData) {

        switch (role) {
        case FilterRole:
            return cacheData.data()->title()
                    + cacheData.data()->getFreeItem();

        case CardRole:
            return  QVariant::fromValue(cacheData.data());

        default:
            return {};
        }
    }

    return {};
}

QHash<int, QByteArray> CardsListModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[CardRole] = "card";

    return roles;
}

const QList<QByteArray> &CardsListModel::cards() const {
    return _cards;
}

void CardsListModel::setCards(const QList<QSharedPointer<Interfaces::iCard> > &newCards) {
    beginResetModel();

    _cache.clear();
    _cards.clear();
    for (const QSharedPointer<Interfaces::iCard>& card : newCards) {
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
CardsListModel::importCard(const QSharedPointer<Interfaces::iCard> &card) {

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
CardsListModel::updateCard(const QSharedPointer<Interfaces::iCard> &card) {

    auto cardModel =  _cache.value(card->cardId());
    if (!cardModel)
        return cardModel;

    cardModel->setCard(card);

    return cardModel;
}

void CardsListModel::updateMetaData(const QList<QSharedPointer<Interfaces::iUsersCards>> &purchasesNumbers) {
    for (const auto &sp:  purchasesNumbers) {
        if (auto model = _cache.value(sp->getCard())) {
            model->setUserData(sp);
        }
    }
}

void CardsListModel::removeCard(const QByteArray& cardId) {

    int index = _cards.indexOf(cardId);

    if (index < 0)
        return;

    beginRemoveRows({}, index, index);

    _cache.remove(cardId);
    _cards.removeAt(index);

    endRemoveRows();

    emit sigCardRemoved(cardId);
}

void CardsListModel::activateCard(const QByteArray& cardId) {
    auto card = _cache.value(cardId);
    if (card) {
        handleActivate(card->card());
    }
}

void CardsListModel::activateCardByIndex(int index) {
    activateCard(_cards.value(index));
}

const QHash<QByteArray, QSharedPointer<CardModel> > &CardsListModel::cache() const {
    return _cache;
}

void CardsListModel::configureModel(const QSharedPointer<CardModel> &cardModel) {
    connect(cardModel.data(), &CardModel::editFinished, this, &CardsListModel::sigEditFinished);
    connect(cardModel.data(), &CardModel::sigRemoveRequest, this, &CardsListModel::sigRemoveRequest);
    connect(cardModel.data(), &CardModel::sigActivate, this, &CardsListModel::handleActivate);
    connect(cardModel.data(), &CardModel::sigShowStatistick, this, &CardsListModel::handleShowStatistic);
    connect(cardModel.data(), &CardModel::resetCardModel, this, &CardsListModel::sigResetCardModel);

}


void CardsListModel::handleActivate(const QSharedPointer<Interfaces::iCard>& card) {
    auto cardModel = _cache.value(card->cardId());

    if (cardModel) {
        emit sigCardSelectedForWork(cardModel);
    }
}

void CardsListModel::handleShowStatistic(const QSharedPointer<Interfaces::iCard>& card) {
    auto cardModel = _cache.value(card->cardId());

    if (cardModel) {
        emit sigCardSelectedForStatistic(cardModel);
    }
}
}
