//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "usermodel.h"
#include "CheatCard/api/api0/user.h"
#include "settingsmodel.h"

namespace RC {

void RC::UserModel::regenerateSessionKey() {
    setSessinon(static_cast<long long >(rand()) * rand());
}

UserModel::UserModel(QSharedPointer<API::User> user) {
    setUser(user);
    srand(time(0));
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

QSharedPointer<API::User> UserModel::user() const {
    return _user;
}

void UserModel::setUser(const QSharedPointer<API::User>& newUser) {

    if (_user == newUser)
        return;

    _user = newUser;


    emit objChanged();

}

API::UserHeader UserModel::getHelloPackage() const {
    API::UserHeader header;
    header.setSessionId(getSessinon());
    header.setUserId(user()->userId());
    header.setToken(user()->getKey());

    auto settings = SettingsModel::instance();
    if (settings && settings->getValue("shareName", true).toBool()) {
        header.setUserName(name());
    }

    return header;
}

void UserModel::handleSettingsChanged(const QString &key, const QVariant &) {
    if (key == "shareName") {
        regenerateSessionKey();
    }
}

const QByteArray &UserModel::sellerToken() const {
    return _sellerToken;
}

void UserModel::setSellerToken(const QByteArray &newSellerToken) {
    _sellerToken = newSellerToken;
    setFSaller(newSellerToken.size());
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

void UserModel::becomeSellerRequest() const {
    emit sigBecomeSeller();
}

QString UserModel::userBackUpPath() const {
    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    return path;
}

QString UserModel::userBackUpData() const {
    return _user->toBytes().toBase64(QByteArray::Base64UrlEncoding);
}

int UserModel::userId() const {
    if (!_user)
        return 0;
    return _user->userId();
}


}
