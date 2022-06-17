//#
//# Copyright (C) 2022-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef USERSLISTMODEL_H
#define USERSLISTMODEL_H

#include <QAbstractListModel>

namespace RC {

class UserModel;

/**
 * @brief The UsersListModel class is list mode l of all availabele on device users.
 */
class UsersListModel: public QAbstractListModel
{
public:
    UsersListModel();


    enum Roles {
        UserObjectRole = Qt::UserRole
    };


    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void activate(int userId);
    void addUser(const QSharedPointer<UserModel>& user);
    void setUsers(const QList<QSharedPointer<UserModel>>& list);

private:
    QHash<unsigned int, QSharedPointer<UserModel>> _cache;
    QList<unsigned int> _users;
};

}
#endif // USERSLISTMODEL_H
