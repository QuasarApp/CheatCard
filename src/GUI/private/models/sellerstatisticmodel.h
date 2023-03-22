//#
//# Copyright (C) 2021-2023 QuasarApp.
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

namespace Interfaces {
class iUsersCards;
class iUser;
}

class CardModel;
class StatistickListProxyModel;


class SellerStatisticModel: public AbstrcattableModelWithHeaders
{
    Q_OBJECT

    Q_PROPERTY(QObject* currentCard READ currentCard NOTIFY currentCardChanged)

public:

    enum Roles {
        SortRole = Qt::UserRole
    };

    SellerStatisticModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QSortFilterProxyModel *proxyModel() const override;

    void setDataList(const QSharedPointer<RC::CardModel>& cardData,
                     const QList<QSharedPointer<RC::Interfaces::iUsersCards> > &newData,
                     const QList<QSharedPointer<RC::Interfaces::iUser>> & userData);

    const QSharedPointer<RC::CardModel>& card() const;

    QObject *currentCard() const;
    void setCurrentCard(const QSharedPointer<CardModel> &newCurrentCard);

    const QHash<QByteArray, QSharedPointer<RC::Interfaces::iUser> > &users() const;
    void setUsers(const QHash<QByteArray, QSharedPointer<RC::Interfaces::iUser> > &newUsers);

    Q_INVOKABLE void chouseRow(int row);
    Q_INVOKABLE double totalValue(int col) const;
signals:
    void currentCardChanged();

private:

    void recalcTotalValues();

    QList<QSharedPointer<RC::Interfaces::iUsersCards>> _data;
    QSharedPointer<RC::CardModel> _card;
    QHash<QByteArray, QSharedPointer<RC::Interfaces::iUser>> _users;

    // where an index of item is column number
    QVector<double> _total;

};

}
#endif // SELLERSTATISTICMODEL_H
