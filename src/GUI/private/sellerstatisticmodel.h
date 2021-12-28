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

namespace API {
class UsersCards;
class User;
}

class CardModel;
class StatistickListProxyModel;


class SellerStatisticModel: public AbstrcattableModelWithHeaders
{
    Q_OBJECT

    Q_PROPERTY(QObject* currentCard READ currentCard NOTIFY currentCardChanged)

public:
    SellerStatisticModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QSortFilterProxyModel *proxyModel() const override;

    void setDataList(const QSharedPointer<RC::CardModel>& cardData,
                     const QList<QSharedPointer<RC::API::UsersCards> > &newData,
                     const QList<QSharedPointer<RC::API::User>> & userData);

    const QSharedPointer<RC::CardModel>& card() const;

    QObject *currentCard() const;
    void setCurrentCard(const QSharedPointer<CardModel> &newCurrentCard);

    const QHash<unsigned int, QSharedPointer<RC::API::User> > &users() const;
    void setUsers(const QHash<unsigned int, QSharedPointer<RC::API::User> > &newUsers);

    Q_INVOKABLE void chouseRow(int row);
signals:
    void currentCardChanged();

private:
    QList<QSharedPointer<RC::API::UsersCards>> _data;
    QSharedPointer<RC::CardModel> _card;
    QHash<unsigned int, QSharedPointer<RC::API::User>> _users;

};

}
#endif // SELLERSTATISTICMODEL_H
