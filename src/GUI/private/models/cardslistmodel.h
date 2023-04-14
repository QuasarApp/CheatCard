//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CARDSLISTMODEL_H
#define CARDSLISTMODEL_H

#include "icardlistmodel.h"

#include <QAbstractListModel>

namespace RC {

namespace Interfaces {
class iCard;
class iUsersCards;
}

class CardModel;

class CardsListModel: public QAbstractListModel, public iCardListModel
{
    Q_OBJECT
public:

    enum Roles {
        FilterRole =  Qt::DisplayRole,
        CardRole = Qt::UserRole,

    };


    enum SelectType {
        ForWork,
        ForStatistick
    };

    CardsListModel();

    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    const QList<QByteArray> &cards() const;
    void setCards(const QList<QSharedPointer<Interfaces::iCard>> &newCards);
    QSharedPointer<CardModel> importCard(const QSharedPointer<Interfaces::iCard> & card);
    void updateMetaData(const QList<QSharedPointer<RC::Interfaces::iUsersCards> > &purchasesNumbers);

    void removeCard(const QByteArray& cardId) override;
    void activateCard(const QByteArray& cardId) override;
    Q_INVOKABLE void activateCardByIndex(int index) override;

    const QHash<QByteArray, QSharedPointer<CardModel> > &cache() const;

signals:
    void sigCardRemoved(QByteArray cardId);
    void sigRemoveRequest(const QSharedPointer<Interfaces::iCard>& card);

    void sigEditFinished(const QSharedPointer<Interfaces::iCard>& card);
    void sigResetCardModel(const QSharedPointer<Interfaces::iCard>& card);
    void sigCardSelectedForWork(const QSharedPointer<RC::CardModel>& card);
    void sigCardSelectedForStatistic(const QSharedPointer<RC::CardModel>& card);

private slots:
    void handleActivate(const QSharedPointer<Interfaces::iCard>& card);
    void handleShowStatistic(const QSharedPointer<Interfaces::iCard>& card);

private:
    QSharedPointer<CardModel> updateCard(const QSharedPointer<Interfaces::iCard> & card);

    void configureModel(const QSharedPointer<CardModel>& cardModel);

    QList<QByteArray> _cards;
    QHash<QByteArray, QSharedPointer<CardModel>> _cache;
};

}
#endif // CARDSLISTMODEL_H
