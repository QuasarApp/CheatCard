#include "cardmodel.h"
#include "rci/objects/iuser.h"
#include "rci/objects/iuserscards.h"
#include "sellerstatisticmodel.h"
#include "statisticklistproxymodel.h"

#include <QDateTime>

namespace RC {

SellerStatisticModel::SellerStatisticModel(QObject *parent):
    AbstrcattableModelWithHeaders(parent) {

    setHHeders(QStringList() <<
               tr("Name") <<
               tr("Id") <<
               tr("Purchases") <<
               tr("Available") <<
               tr("Issued") <<
               tr("Last visit") <<
               tr("Status")
               );
}

int SellerStatisticModel::rowCount(const QModelIndex &) const {
    return _data.size();
}

int SellerStatisticModel::columnCount(const QModelIndex &) const {
    return Hheders().size();
}

QVariant SellerStatisticModel::data(const QModelIndex &index, int role) const {

    if (index.row() >= _data.size()) {
        return unknownValue();
    }

    const QSharedPointer<RC::Interfaces::iUsersCards> &item = _data.at(index.row());
    const QSharedPointer<RC::Interfaces::iUser> &user = _users.value(item->getUser());


    if (role == Qt::DisplayRole || role == SortRole) {
        switch (index.column()) {
        case 0 : {
            if (!user)
                return unknownValue();

            return user->name();
        }

        case 1 : {
            return QString("id:%0").arg(item->getUser());
        }

        case 2 : {
            return item->getPurchasesNumber();
        }

        case 3 : {
            return CardModel::availableItems(item, _card->card());
        }

        case 4 : {
            return item->getReceived();
        }

        case 5 : {
            if (role == SortRole) {
                return item->getRawTime();
            }
            return QDateTime::fromSecsSinceEpoch(item->getRawTime());
        }

        case 6 : {
            if (item->isActive()) {
                return tr("Active");
            }
            return tr("Passive");
        }

        default:
            return unknownValue();
        }
    }

    return unknownValue();
}

QSortFilterProxyModel *SellerStatisticModel::proxyModel() const {
    return new StatistickListProxyModel();
}

void SellerStatisticModel::setDataList(const QSharedPointer<CardModel> &cardData,
                                       const QList<QSharedPointer<RC::Interfaces::iUsersCards> > &newData,
                                       const QList<QSharedPointer<RC::Interfaces::iUser>> & userData) {

    beginResetModel();
    setCurrentCard(cardData);
    _data = newData;

    for (const auto &user: qAsConst(userData)) {
        _users[user->id()] = user;
    }

    recalcTotalValues();
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

const QHash<unsigned int, QSharedPointer<RC::Interfaces::iUser> > &SellerStatisticModel::users() const {
    return _users;
}

void SellerStatisticModel::setUsers(const QHash<unsigned int, QSharedPointer<RC::Interfaces::iUser> > &newUsers) {
    _users = newUsers;
}

void SellerStatisticModel::chouseRow(int row) {
    if (_card) {
        _card->setUserData(_data.value(row, nullptr));
    }
}

double SellerStatisticModel::totalValue(int col) const {
    return _total.value(col, 0);
}

void SellerStatisticModel::recalcTotalValues() {
    _total.clear();
    _total.resize(columnCount());

    for (int col = 0; col < columnCount(); ++col) {
        for (int row = 0; row < rowCount(); ++row) {
            QVariant val = data(index(row, col), Qt::DisplayRole);
            bool ok = false;
            double dval = val.toDouble(&ok);

            if (!ok) {
                break;
            }

            _total[col] += dval;
        }
    }

}

}
