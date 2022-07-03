#include "userslistmodel.h"
#include "usermodel.h"

#include <CheatCard/api/api0/user.h>
#include <qaglobalutils.h>
#include "imagesstoragemodel.h"

namespace RC {


UsersListModel::UsersListModel(ImagesStorageModel *imageStorage) {
    _defaultAvatars = imageStorage;
    debug_assert(_defaultAvatars, "avatars object should be initialized!");
}

int RC::UsersListModel::rowCount(const QModelIndex &) const {
    return _users.size();
}

QVariant RC::UsersListModel::data(const QModelIndex &index, int role) const {

    if (index.row() >= rowCount()) {
        return {};
    }

    unsigned int userId = _users[index.row()];

    if (role == UserId)
        return userId;

    auto cacheData = _cache.value(userId, {});

    if (cacheData) {
        return QVariant::fromValue(cacheData.data());
    }

    return {};
}

QString UsersListModel::userDefaultAvatar(int userId) {
    return _defaultAvatars->getImageByHash(userId);
}

QHash<int, QByteArray> RC::UsersListModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[UserObjectRole] = "userObject";
    roles[UserId] = "userID";

    return roles;

}

void UsersListModel::setUsers(const QList<QSharedPointer<API::User> >
                              &newUsers) {
    beginResetModel();

    _cache.clear();

    for (const QSharedPointer<API::User>& user : newUsers) {
        unsigned int id = user->getId().toUInt();

        auto userModel =  QSharedPointer<UserModel>::create(user);
        _cache.insert(id,
                      userModel
                      );
        _users.push_back(id);

    }

    endResetModel();

}

QSharedPointer<UserModel>
UsersListModel::importUser(const QSharedPointer<API::User> &user) {

    if (!user)
        return {};

    unsigned int id = user->getId().toUInt();

    if (_users.contains(id)) {
        return updateUser(user);
    }

    auto userModel = QSharedPointer<UserModel>::create(user);

    beginInsertRows({}, _users.size(), _users.size());

    _cache.insert(id, userModel);
    _users.push_back(id);

    endInsertRows();

    return userModel;
}

QSharedPointer<UserModel>
UsersListModel::updateUser(const QSharedPointer<API::User> &user) {

    unsigned int id = user->getId().toUInt();

    auto userModel =  _cache.value(id);
    if (!userModel)
        return userModel;

    userModel->setUser(user);

    return userModel;
}

void UsersListModel::setCurrentUser(unsigned int newCurrentUser) {

    if (newCurrentUser == _currentUser) {
        return;
    }

    if (_cache.contains(newCurrentUser)) {
        _currentUser = newCurrentUser;
    } else if (_cache.size()) {
        _currentUser = _cache.begin().key();
    } else {
        return;
    }

    emit sigUserChanged(currentUser());
    emit currentUserIdChanged();
}

const QHash<unsigned int, QSharedPointer<UserModel> > &
UsersListModel::cache() const {
    return _cache;
}

QSharedPointer<UserModel> UsersListModel::currentUser() const {
    return _cache.value(_currentUser, nullptr);
}

int UsersListModel::currentUserId() const {
    return _currentUser;
}

QObject *UsersListModel::currentUserModel() const {
    return currentUser().data();
}

}
