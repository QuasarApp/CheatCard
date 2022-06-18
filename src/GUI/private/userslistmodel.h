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
class ImagesStorageModel;
namespace API {
    class User;
};

/**
 * @brief The UsersListModel class is list mode l of all availabele on device users.
 */
class UsersListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    UsersListModel(ImagesStorageModel * imageStorage);


    enum Roles {
        UserObjectRole = Qt::UserRole,
        UserId

    };


    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE QString userDefaultAvatar(int userId);

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void activate(int userId);
    void setUsers(const QList<QSharedPointer<API::User>>& list);

    QSharedPointer<UserModel>
    importUser(const QSharedPointer<API::User>& user);

signals:
    void sigUserChanged(const QSharedPointer<RC::UserModel>& newUser);

private:
    QSharedPointer<UserModel>
    updateUser(const QSharedPointer<API::User>& user);

    QHash<unsigned int, QSharedPointer<UserModel>> _cache;
    QList<unsigned int> _users;
    ImagesStorageModel * _defaultAvatars = nullptr;
};

}
#endif // USERSLISTMODEL_H
