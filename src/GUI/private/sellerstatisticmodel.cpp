#include "cardmodel.h"
#include "sellerstatisticmodel.h"
#include "statisticklistproxymodel.h"

#include <CheatCard/user.h>
#include <CheatCard/userscards.h>

namespace RC {

SellerStatisticModel::SellerStatisticModel(QObject *parent):
    AbstrcattableModelWithHeaders(parent) {

    setHHeders(QStringList() <<
               tr("Name") <<
               tr("Id") <<
               tr("Purchases") <<
               tr("Issued") <<
               tr("Last visit")
               );

    _proxy = new StatistickListProxyModel(this);
    _proxy->setDynamicSortFilter(true);
    _proxy->setSourceModel(this);
    _proxy->setFilterKeyColumn(0);
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
        const QSharedPointer<RC::User> &user = _users.value(item->getUser());

        switch (index.column()) {
        case 0 : {
            if (!user)
                return unknownValue();

            return user->name();
        }

        case 1 : {
            return item->getUser();
        }

        case 2 : {
            return item->getPurchasesNumber();
        }

        case 3 : {
            return item->getReceived();
        }

        case 4 : {            
            return  item->getTime().toString();
        }

        default:
            return unknownValue();
        }
    }

    return unknownValue();
}

void SellerStatisticModel::setDataList(const QSharedPointer<CardModel> &cardData,
                                       const QList<QSharedPointer<RC::UsersCards> > &newData,
                                       const QList<QSharedPointer<RC::User>> & userData) {

    beginResetModel();
    setCurrentCard(cardData);
    _data = newData;

    for (const auto &user: qAsConst(userData)) {
        _users[user->userId()] = user;
    }

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

const QHash<unsigned int, QSharedPointer<RC::User> > &SellerStatisticModel::users() const {
    return _users;
}

void SellerStatisticModel::setUsers(const QHash<unsigned int, QSharedPointer<RC::User> > &newUsers) {
    _users = newUsers;
}

}
