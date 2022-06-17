#include "userslistmodel.h"
#include "usermodel.h"

#include <CheatCard/api/api0/user.h>
#include

namespace RC {
UsersListModel::UsersListModel() {

}

int RC::UsersListModel::rowCount(const QModelIndex &) const {
    return _users.size();
}

QVariant RC::UsersListModel::data(const QModelIndex &index, int role) const {
    if (role != UserObjectRole) {
        return {};
    }

    if (index.row() >= rowCount()) {
        return {};
    }

    unsigned int userId = _users[index.row()];

    auto cacheData = _cache.value(userId, {});

    if (cacheData) {
        return QVariant::fromValue(cacheData.data());
    }

    return {};
}

QHash<int, QByteArray> RC::UsersListModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[UserObjectRole] = "userObject";

    return roles;

}

void UsersListModel::activate(int userId) {
// to do
}

void UsersListModel::addUser(const QSharedPointer<UserModel> &user) {
    beginInsertRows({}, _users.size(), _users.size());

    unsigned int id = user->user()->getId().toUInt();
    _cache.insert(id, user);
    _users.push_back(id);

    endInsertRows();

}

void UsersListModel::setUsers(const QList<QSharedPointer<UserModel> > &list) {
    beginResetModel();
    _cache.clear();
    endResetModel();

}
}
