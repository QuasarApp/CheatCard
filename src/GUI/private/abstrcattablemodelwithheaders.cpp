#include "abstrcattablemodelwithheaders.h"
namespace RC {

AbstrcattableModelWithHeaders::AbstrcattableModelWithHeaders(QObject *parent):
    QAbstractTableModel(parent) {
    _unknownValue = tr("Unknown");
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

