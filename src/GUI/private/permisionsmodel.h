//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef PERMISIONSMODEL_H
#define PERMISIONSMODEL_H

#include "abstrcattablemodelwithheaders.h"
#include <CheatCard/api/api0/contacts.h>
#include <QSharedPointer>

namespace RC {

/**
 * @brief The PermisioonsModel class
 */
class PermisionsModel: public QAbstractListModel
{
    Q_OBJECT
public:
    PermisionsModel();

    enum Roles {
        PermisionRole = Qt::UserRole
    };

    int rowCount(const QModelIndex &parent = {}) const override;
    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE void setNewDescription(int row, const QString& description);
    void setPermissions(const QList<QSharedPointer<API::Contacts> > &newData);

signals:
    void sigPermision(const QSharedPointer<RC::API::Contacts>& permision);
    void sigPermisionRemoved(QSharedPointer<RC::API::Contacts> permision);

private:
    QHash<unsigned int, QSharedPointer<RC::API::Contacts>> _data;
    QList<unsigned int> _permissions;

};

}
#endif // PERMISIONSMODEL_H
