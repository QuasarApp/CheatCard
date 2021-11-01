//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CARDSLISTMODEL_H
#define CARDSLISTMODEL_H

#include <QAbstractListModel>
#include <CheatCard/database.h>

namespace RC {

class CardModel;
class Card;
class UsersCards;

class CardsListModel: public QAbstractListModel
{
    Q_OBJECT
public:

    enum Roles {
        CardRole = Qt::UserRole
    };

    CardsListModel();

    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    const QList<int> &cards() const;
    void setCards(const QList<QSharedPointer<Card>> &newCards);
    QSharedPointer<CardModel> importCard(const QSharedPointer<Card> & card);
    void setPurchasesNumbers(const QList<QSharedPointer<RC::UsersCards> > &purchasesNumbers);

    Q_INVOKABLE void addCard();
    Q_INVOKABLE void removeCard(int cardId);
    Q_INVOKABLE void cardSelected(int cardId);

    const QHash<int, QSharedPointer<CardModel>> &cache() const;

signals:
    void sigCardRemoved(int cardName);
    void sigEditFinished(const QSharedPointer<Card>& card);
    void sigCardSelectedForWork(const QSharedPointer<CardModel>& card);


private:
    QSharedPointer<CardModel> updateCard(const QSharedPointer<Card> & card);

    void configureModel(const QSharedPointer<CardModel>& cardModel);

    QHash<int, QSharedPointer<CardModel>> _cache;

    QList<int> _cards;
};

}
#endif // CARDSLISTMODEL_H
