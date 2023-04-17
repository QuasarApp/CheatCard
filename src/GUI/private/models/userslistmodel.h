//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef USERSLISTMODEL_H
#define USERSLISTMODEL_H

#include "basemodel.h"

#include <QAbstractListModel>

namespace RC {

class UserModel;
class ImagesStorageModel;
namespace Interfaces {
    class iUser;
};

/**
 * @brief The UsersListModel class is list mode l of all availabele on device users.
 */
class UsersListModel: public QAbstractListModel, public BaseModel
{
    Q_OBJECT
    Q_PROPERTY(int usersCount READ usersCount NOTIFY usersCountChanged)
    Q_PROPERTY(QByteArray currentUserKey READ currentUserKey WRITE setCurrentUser NOTIFY currentUserKeyChanged)
    Q_PROPERTY(QObject* currentUserModel READ currentUserModel NOTIFY currentUserKeyChanged)

public:

    UsersListModel();


    enum Roles {
        UserObjectRole = Qt::UserRole,
        Row,
        DefaultAvatar,
        IsCurrent

    };


    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setUsers(const QList<QSharedPointer<Interfaces::iUser>>& list);

    QSharedPointer<UserModel>
    importUser(const QSharedPointer<Interfaces::iUser>& user);
    Q_INVOKABLE void removeUser(const QByteArray &userId);

    const QHash<QByteArray, QSharedPointer<UserModel> > &
    cache() const;
    const QSharedPointer<UserModel>& currentUser() const;

    void setCurrentUser(const QByteArray& newCurrentUser);
    void setCurrentUser(const QSharedPointer<Interfaces::iUser> &newCurrentUser);

    QByteArray currentUserKey() const;
    QByteArray currentUserSecret() const;

    QObject *currentUserModel() const;

    int usersCount() const;

    void init(const QSharedPointer<Interfaces::iDB> &db,
              const QSharedPointer<Interfaces::iModelsStorage> &global) override;

    void saveCurrentUser();


signals:
    void sigUserChanged(const QSharedPointer<RC::UserModel>& newUser);
    void currentUserKeyChanged(const QByteArray& key);
    void usersCountChanged();

private slots:
    void handleUserEditFinished();

private:
    QSharedPointer<UserModel>
    updateUser(const QSharedPointer<Interfaces::iUser>& user);
    void setCurrentUserPrivate(const QSharedPointer<UserModel> &newCurrentUser);

    QSharedPointer<UserModel> _currentUser;

    QHash<QByteArray, QSharedPointer<UserModel>> _cache;
    QList<QByteArray> _users;
    QSharedPointer<ImagesStorageModel> _defaultAvatars;

};

}
#endif // USERSLISTMODEL_H
