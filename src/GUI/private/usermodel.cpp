#include "usermodel.h"
#include "CheatCard/user.h"

#include <CheatCard/datastructures.h>
namespace RC {

UserModel::UserModel(QSharedPointer<User> user) {
    setUser(user);

    srand(time(0));
    setSessinon(static_cast<long long >(rand()) * rand());
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

UserHeader UserModel::getHelloPackage() const {
    UserHeader header;
    header.setSessionId(getSessinon());
    header.setUserId(user()->userId());
    header.setToken(user()->getKey());

    return header;
}

long long UserModel::getSessinon() const {
    return sessinon;
}

void UserModel::setSessinon(long long newSessinon) {
    if (sessinon == newSessinon)
        return;
    sessinon = newSessinon;
    _sessionCode = getHelloPackage().toBytes().toHex();

    emit sessinonChanged();
}

const QString &UserModel::sessionCode() const {
    return _sessionCode;
}

}
