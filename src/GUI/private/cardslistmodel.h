//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CARDSLISTMODEL_H
#define CARDSLISTMODEL_H

#include "icardlistmodel.h"

#include <QAbstractListModel>
#include <CheatCard/database.h>

namespace RC {

class CardModel;
class Card;
class UsersCards;

class CardsListModel: public QAbstractListModel, public iCardListModel
{
    Q_OBJECT
public:

    enum Roles {
        CardRole = Qt::UserRole
    };


    enum SelectType {
        ForWork,
        ForStatistick
    };

    CardsListModel();

    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    const QList<int> &cards() const;
    void setCards(const QList<QSharedPointer<Card>> &newCards);
    QSharedPointer<CardModel> importCard(const QSharedPointer<Card> & card);
    void updateMetaData(const QList<QSharedPointer<RC::UsersCards> > &purchasesNumbers);

    Q_INVOKABLE void addCard() override;
    Q_INVOKABLE void removeCard(int cardId) override;
    Q_INVOKABLE void cardSelected(int cardId, int type) override;

    const QHash<int, QSharedPointer<CardModel>> &cache() const;

signals:
    void sigCardRemoved(unsigned int cardName);
    void sigEditFinished(const QSharedPointer<Card>& card);
    void sigCardSelectedForWork(const QSharedPointer<CardModel>& card);
    void sigCardSelectedForStatistic(const QSharedPointer<CardModel>& card);


private:
    QSharedPointer<CardModel> updateCard(const QSharedPointer<Card> & card);

    void configureModel(const QSharedPointer<CardModel>& cardModel);

    QHash<int, QSharedPointer<CardModel>> _cache;

    QList<int> _cards;
};

}
#endif // CARDSLISTMODEL_H
