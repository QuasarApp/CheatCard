//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef ABSTRCATTABLEMODELWITHHEADERS_H
#define ABSTRCATTABLEMODELWITHHEADERS_H

#include <QAbstractTableModel>

namespace RC {

class AbstrcattableModelWithHeaders: public QAbstractTableModel
{
    Q_OBJECT
public:
    AbstrcattableModelWithHeaders(QObject* parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int) const override;

    const QStringList &Hheders() const;
    void setHHeders(const QStringList &newHeders);

    const QStringList &Vheders() const;
    void setVheders(const QStringList &newVheders);

    const QString &unknownValue() const;

private:

    QString _unknownValue;

    QStringList _Hheders;
    QStringList _Vheders;
};
}
#endif // ABSTRCATTABLEMODELWITHHEADERS_H
