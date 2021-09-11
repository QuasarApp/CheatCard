#include "usermodel.h"
#include "user.h"
namespace RC {

UserModel::UserModel(QSharedPointer<User> user) {
    setUser(user);
}

bool UserModel::fSaller() const {
    if (!_user)
        return false;
    return _user->fSaller();
}

void UserModel::setFSaller(bool newFSaller) {
    if (!_user)
        return;

    if (_user->fSaller() == newFSaller)
        return;

    _user->setFSaller(newFSaller);

    emit objChanged();
}

QString UserModel::name() const {
    if (!_user)
        return "Unknown";
    return _user->name();
}

void UserModel::setName(const QString &newName) {
    if (!_user)
        return;

    if (_user->name() == newName)
        return;

    _user->setName(newName);

    emit objChanged();
}

QSharedPointer<User> UserModel::user() const {
    return _user;
}

void UserModel::setUser(const QSharedPointer<User>& newUser) {

    if (_user == newUser)
        return;

    _user = newUser;
    emit objChanged();

}
}
