//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CARDSLISTMODEL_H
#define CARDSLISTMODEL_H

#include <QAbstractListModel>
#include "database.h"

namespace RC {

class CardModel;
class Card;

struct TableCache {

    TableCache ();

    TableCache (const QSharedPointer<Card>& src,
                const QSharedPointer<CardModel>& mod);

    QSharedPointer<Card> source;
    QSharedPointer<CardModel> model;

    bool isValid() const;
};

class CardsListModel: public QAbstractListModel
{
    Q_OBJECT
public:

    enum Roles {
        CardRole = Qt::UserRole
    };

    CardsListModel(DB* db);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    const QList<int> &cards() const;
    void setCards(const QHash<int, QSharedPointer<Card>> &newCards);

private:
    QHash<int, TableCache> _cache;
    DB * _db = nullptr;

    QList<int> _cards;
};

}
#endif // CARDSLISTMODEL_H
