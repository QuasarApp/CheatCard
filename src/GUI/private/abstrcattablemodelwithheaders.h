//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef ABSTRCATTABLEMODELWITHHEADERS_H
#define ABSTRCATTABLEMODELWITHHEADERS_H

#include <QAbstractTableModel>

class QSortFilterProxyModel;

namespace RC {

class AbstrcattableModelWithHeaders: public QAbstractTableModel
{
    Q_OBJECT
public:
    AbstrcattableModelWithHeaders(QObject* parent = nullptr);
    ~AbstrcattableModelWithHeaders() override;

    QVariant headerData(int section, Qt::Orientation orientation, int) const override;

    const QStringList &Hheders() const;
    void setHHeders(const QStringList &newHeders);

    const QStringList &Vheders() const;
    void setVheders(const QStringList &newVheders);

    const QString &unknownValue() const;

    virtual QSortFilterProxyModel* proxyModel() const;

    Q_INVOKABLE QObject* proxy();

    Q_INVOKABLE void sortView(int column);

private:

    QString _unknownValue;

    QStringList _Hheders;
    QStringList _Vheders;

    QSortFilterProxyModel *_proxy = nullptr;

};
}
#endif // ABSTRCATTABLEMODELWITHHEADERS_H
