#include "userslistmodel.h"
#include "rci/core/imodelsstorage.h"
#include "rci/objects/iuser.h"
#include "imagelogomodel.h"
#include "usermodel.h"

#include <qaglobalutils.h>
#include "imagesstoragemodel.h"

namespace RC {


UsersListModel::UsersListModel() {
}

int RC::UsersListModel::rowCount(const QModelIndex &) const {
    return _users.size();
}

QVariant RC::UsersListModel::data(const QModelIndex &index, int role) const {
    if (role == Row) {
        return index.row();
    }

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
    if (!_defaultAvatars)
        return "";

    return _defaultAvatars->getImageByHash(userId);
}

QHash<int, QByteArray> RC::UsersListModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[UserObjectRole] = "userObject";
    roles[UserId] = "userID";
    roles[Row] = "rowNumber";

    return roles;

}

void UsersListModel::setUsers(const QList<QSharedPointer<Interfaces::iUser> >
                              &newUsers) {
    beginResetModel();

    _cache.clear();

    for (const QSharedPointer<Interfaces::iUser>& user : newUsers) {

        auto userModel =  QSharedPointer<UserModel>::create(user);
        _cache.insert(user->id(),
                      userModel
                      );
        _users.push_back(user->id());

    }

    endResetModel();
    emit usersCountChanged();

}

QSharedPointer<UserModel>
UsersListModel::importUser(const QSharedPointer<Interfaces::iUser> &user) {

    if (!user)
        return {};

    unsigned int id = user->id();

    if (_users.contains(id)) {
        return updateUser(user);
    }

    auto userModel = QSharedPointer<UserModel>::create(user);

    beginInsertRows({}, _users.size(), _users.size());

    _cache.insert(id, userModel);
    _users.push_back(id);

    endInsertRows();

    emit usersCountChanged();

    return userModel;
}

void UsersListModel::removeUser(int userId) {
    int index = _users.indexOf(userId);

    if (index < 0)
        return;

    beginRemoveRows({}, index, index);

    _cache.remove(userId);
    _users.removeAt(index);

    endRemoveRows();

    emit usersCountChanged();
}

QSharedPointer<UserModel>
UsersListModel::updateUser(const QSharedPointer<Interfaces::iUser> &user) {

    unsigned int id = user->id();

    auto userModel =  _cache.value(id);
    if (!userModel)
        return userModel;

    userModel->setUser(user);

    return userModel;
}

void UsersListModel::init(const QSharedPointer<Interfaces::iDB> &db,
                          const QSharedPointer<Interfaces::iModelsStorage> &global) {
    BaseModel::init(db, global);
    _defaultAvatars = global->get(typeid(ImageLogoModel).hash_code()).staticCast<ImagesStorageModel>();
}

void UsersListModel::setCurrentUser(unsigned int newCurrentUser) {

    if (_currentUser && newCurrentUser == _currentUser) {
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

int UsersListModel::usersCount() const {
    return rowCount();
}

}
