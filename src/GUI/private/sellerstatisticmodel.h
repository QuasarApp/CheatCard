//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SELLERSTATISTICMODEL_H
#define SELLERSTATISTICMODEL_H

#include "abstrcattablemodelwithheaders.h"
#include <QSharedPointer>
#include <QList>

namespace RC {

class UsersCards;
class CardModel;
class StatistickListProxyModel;
class User;

class SellerStatisticModel: public AbstrcattableModelWithHeaders
{
    Q_OBJECT

    Q_PROPERTY(QObject* currentCard READ currentCard NOTIFY currentCardChanged)

public:
    SellerStatisticModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    void setDataList(const QSharedPointer<RC::CardModel>& cardData,
                     const QList<QSharedPointer<RC::UsersCards> > &newData,
                     const QList<QSharedPointer<RC::User>> & userData);

    const QSharedPointer<RC::CardModel>& card() const;

    QObject *currentCard() const;
    void setCurrentCard(const QSharedPointer<CardModel> &newCurrentCard);

    Q_INVOKABLE QObject* proxy() const;

    const QHash<unsigned int, QSharedPointer<RC::User> > &users() const;
    void setUsers(const QHash<unsigned int, QSharedPointer<RC::User> > &newUsers);

signals:
    void currentCardChanged();

private:
    QList<QSharedPointer<RC::UsersCards>> _data;
    QSharedPointer<RC::CardModel> _card;
    QHash<unsigned int, QSharedPointer<RC::User>> _users;

    StatistickListProxyModel *_proxy = nullptr;
};

}
#endif // SELLERSTATISTICMODEL_H
