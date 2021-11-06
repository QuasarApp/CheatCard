#include "abstrcattablemodelwithheaders.h"

#include <qsortfilterproxymodel.h>
namespace RC {

AbstrcattableModelWithHeaders::AbstrcattableModelWithHeaders(QObject *parent):
    QAbstractTableModel(parent) {
    _unknownValue = tr("Unknown");
}

AbstrcattableModelWithHeaders::~AbstrcattableModelWithHeaders() {
    if (_proxy)
        delete _proxy;
}

QVariant AbstrcattableModelWithHeaders::headerData(int section,
                                                   Qt::Orientation orientation,
                                                   int role) const {

    if (role == Qt::DisplayRole) {
        const auto *headersAdday = &Vheders();
        if (orientation == Qt::Orientation::Horizontal) {
            headersAdday = &Hheders();
        }

        return headersAdday->value(section, unknownValue());
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

const QString &AbstrcattableModelWithHeaders::unknownValue() const {
    return _unknownValue;
}

QSortFilterProxyModel *AbstrcattableModelWithHeaders::proxyModel() const {
    return new QSortFilterProxyModel();
}

QObject *AbstrcattableModelWithHeaders::proxy() {

    if (!_proxy) {
        _proxy = proxyModel();
        _proxy->setDynamicSortFilter(true);
        _proxy->setSourceModel(this);
        _proxy->setFilterKeyColumn(0);
    }

    return _proxy;
}

void AbstrcattableModelWithHeaders::sortView(int column, int role) {
    if (_proxy) {
        _proxy->setFilterKeyColumn(column);
        _proxy->setSortRole(role);
        _proxy->sort(column, static_cast<Qt::SortOrder>((_proxy->sortOrder() + 1) % 2));
    }
}

const QStringList &AbstrcattableModelWithHeaders::Hheders() const {
    return _Hheders;
}

void AbstrcattableModelWithHeaders::setHHeders(const QStringList &newHeders) {
    beginResetModel();
    _Hheders = newHeders;
    endResetModel();

}

const QStringList &AbstrcattableModelWithHeaders::Vheders() const {
    return _Vheders;
}

void AbstrcattableModelWithHeaders::setVheders(const QStringList &newVheders) {
    beginResetModel();
    _Vheders = newVheders;
    endResetModel();
}

}

