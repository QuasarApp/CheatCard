//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef PILLSMODEL_H
#define PILLSMODEL_H

#include <QAbstractListModel>
#include <doctor.h>

namespace RC {

/**
 * @brief The PillsModel class This is gui model of available pills.
 */
class PillsModel: public QAbstractListModel
{
    Q_OBJECT

    enum Roles {
        Name = Qt::UserRole,
        Description
    };

public:
    PillsModel();
    void addPill(const QSharedPointer<QuasarAppUtils::iPill>& pill);

    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void usePill(int index);

private slots:
    void handleFixFailed(QList<QSharedPointer<QuasarAppUtils::iPill>>) const;
    void handleFixSuccessful(QList<QSharedPointer<QuasarAppUtils::iPill>>) const;
    void handleBugDetected(QList<QSharedPointer<QuasarAppUtils::iPill>>);

private:
    QuasarAppUtils::Doctor _doctor;
    QHash<QString, QSharedPointer<QuasarAppUtils::iPill>> _viewData;
};

}
#endif // PILLSMODEL_H
