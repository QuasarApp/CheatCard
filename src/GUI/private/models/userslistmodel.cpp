#include "userslistmodel.h"
#include "rci/core/imodelsstorage.h"
#include "rci/objects/iuser.h"
#include "imagelogomodel.h"
#include "usermodel.h"

#include <qaglobalutils.h>
#include "imagesstoragemodel.h"
#include <rci/core/idb.h>

namespace RC {


UsersListModel::UsersListModel() {
}

int RC::UsersListModel::rowCount(const QModelIndex &) const {
    return _users.size();
}

QVariant RC::UsersListModel::data(const QModelIndex &index, int role) const {

    if (index.row() >= rowCount()) {
        return {};
    }

    switch (role) {
    case Row: {
        return index.row();
    }

    case DefaultAvatar: {
        if (!_defaultAvatars)
            return "";

        auto userKey = _users.value(index.row());
        return _defaultAvatars->getImageByHash(userKey);
    }

    case UserObjectRole: {
        auto userKey = _users.value(index.row());
        return QVariant::fromValue(_cache.value(userKey).data());
    }

    case IsCurrent: {
        auto userKey = _users.value(index.row());
        return userKey == currentUserKey();
    }

    default:
        return {};
    }
}

QHash<int, QByteArray> RC::UsersListModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[UserObjectRole] = "userObject";
    roles[Row] = "rowNumber";
    roles[DefaultAvatar] = "defaultAvatar";
    roles[IsCurrent] = "isCurrent";

    return roles;

}

void UsersListModel::setUsers(const QList<QSharedPointer<Interfaces::iUser> >
                                  &newUsers) {
    beginResetModel();

    _cache.clear();

    for (const QSharedPointer<Interfaces::iUser>& user : newUsers) {

        auto userModel =  QSharedPointer<UserModel>::create(user);
        _cache.insert(user->getKey(),
                      userModel
                      );
        _users.push_back(user->getKey());

    }

    endResetModel();
    emit usersCountChanged();

}

QSharedPointer<UserModel>
UsersListModel::importUser(const QSharedPointer<Interfaces::iUser> &user) {

    if (!user)
        return {};

    auto key = user->getKey();

    if (_users.contains(key)) {
        return updateUser(user);
    }

    auto userModel = QSharedPointer<UserModel>::create(user);

    beginInsertRows({}, _users.size(), _users.size());

    _cache.insert(key, userModel);
    _users.push_back(key);

    endInsertRows();

    emit usersCountChanged();

    return userModel;
}

void UsersListModel::removeUser(const QByteArray& userId) {
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

    auto id = user->getKey();

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

void UsersListModel::saveCurrentUser() {
    handleUserEditFinished();
}

void UsersListModel::handleUserEditFinished() {
    if (auto _db = db()) {
        if (auto userModel = static_cast<UserModel*>(sender())) {
            auto user = userModel->user();
            _db->saveUser(user);
        }
    }
}

void UsersListModel::setCurrentUser(const QByteArray &newCurrentUser) {

    if (_currentUser && newCurrentUser == _currentUser->userKey()) {
        return;
    }

    auto user = _cache.value(newCurrentUser);
    if (!user)
        user = *_cache.begin();

    if (!user)
        return;

    setCurrentUserPrivate(user);
}

void UsersListModel::setCurrentUser(const QSharedPointer<Interfaces::iUser> &newCurrentUser) {

    if (!_cache.contains(newCurrentUser->getKey())) {
        setCurrentUserPrivate(importUser(newCurrentUser));
    } else {
        setCurrentUserPrivate(_cache.value(newCurrentUser->getKey()));
    }
}

void UsersListModel::setCurrentUserPrivate(const QSharedPointer<UserModel> &newCurrentUser) {

    if (_currentUser) {
        disconnect(_currentUser.data(), &UserModel::objChanged,
                   this, &UsersListModel::handleUserEditFinished);
    }

    _currentUser = newCurrentUser;

    if (_currentUser) {
        connect(_currentUser.data(), &UserModel::objChanged,
                this, &UsersListModel::handleUserEditFinished);
    }

    emit sigUserChanged(currentUser());
    emit currentUserKeyChanged(currentUser()->userKey());
}

const QHash<QByteArray, QSharedPointer<UserModel> > &UsersListModel::cache() const {
    return _cache;
}

const QSharedPointer<UserModel> &UsersListModel::currentUser() const {
    return _currentUser;
}

QByteArray UsersListModel::currentUserKey() const {
    if (_currentUser && _currentUser->user()) {
        return _currentUser->user()->getKey();
    }

    return {};
}

QByteArray UsersListModel::currentUserSecret() const {
    if (_currentUser && _currentUser->user()) {
        return _currentUser->user()->secret();
    }

    return {};
}

QObject *UsersListModel::currentUserModel() const {
    return currentUser().data();
}

int UsersListModel::usersCount() const {
    return rowCount();
}

}
