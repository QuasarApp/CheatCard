#include "cardmodel.h"
#include "sellerstatisticmodel.h"
#include "statisticklistproxymodel.h"

#include <CheatCard/userscards.h>

namespace RC {

SellerStatisticModel::SellerStatisticModel(QObject *parent):
    AbstrcattableModelWithHeaders(parent) {

    setHHeders(QStringList() <<
               tr("Name") <<
               tr("Purchases count") <<
               tr("Total bonuses issued"));

    _proxy = new StatistickListProxyModel(this);
    _proxy->setDynamicSortFilter(true);
    _proxy->setSourceModel(this);
}

int SellerStatisticModel::rowCount(const QModelIndex &) const {
    return _data.size();
}

int SellerStatisticModel::columnCount(const QModelIndex &) const {
    return Hheders().size();
}

QVariant SellerStatisticModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {

        if (index.row() >= _data.size()) {
            return unknownValue();
        }

        const QSharedPointer<RC::UsersCards> &item = _data.at(index.row());

        switch (index.column()) {
        case 0 : {
            return item->getUser();
        }
        case 1 : {
            return item->getPurchasesNumber();
        }
        case 2 : {
            return item->getReceived();
        }
        default:
            return unknownValue();
        }
    }

    return unknownValue();
}

void SellerStatisticModel::setDataList(const QSharedPointer<CardModel> &cardData,
                                       const QList<QSharedPointer<RC::UsersCards> > &newData) {

    beginResetModel();
    setCurrentCard(cardData);
    _data = newData;
    endResetModel();
}

const QSharedPointer<CardModel> &SellerStatisticModel::card() const {
    return _card;
}

QObject *SellerStatisticModel::currentCard() const {
    return _card.data();
}

void SellerStatisticModel::setCurrentCard(const QSharedPointer<CardModel> &newCurrentCard) {
    if (_card == newCurrentCard)
        return;

    _card = newCurrentCard;
    emit currentCardChanged();
}

QObject *SellerStatisticModel::proxy() const {
    return _proxy;
}

}
