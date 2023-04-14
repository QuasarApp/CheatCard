//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "usermodel.h"
#include "CheatCard/userheader.h"
#include "qqmlengine.h"
#include "rci/objects/iuser.h"
#include "settingsmodel.h"

#include <QStandardPaths>

namespace RC {

UserModel::UserModel(QSharedPointer<Interfaces::iUser> user) {
    setUser(user);
    QQmlEngine::setObjectOwnership(this,
                                   QQmlEngine::CppOwnership);
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

QSharedPointer<Interfaces::iUser> UserModel::user() const {
    return _user;
}

void UserModel::setUser(const QSharedPointer<Interfaces::iUser>& newUser) {

    if (_user == newUser)
        return;

    _user = newUser;

    setSessionCode(getHelloPackage().toBytes().toHex());
    emit objChanged();

}

UserHeader UserModel::getHelloPackage() const {
    UserHeader header;
    header.setUserKey(user()->getKey());

    auto settings = SettingsModel::instance();
    if (settings && settings->getValue("shareName", true).toBool()) {
        header.setUserName(name());
    }

    return header;
}

void UserModel::handleSettingsChanged(const QString &key, const QVariant &) {
    if (key == "shareName") {
        setSessionCode(getHelloPackage().toBytes().toHex());
    }
}

void UserModel::setSessionCode(const QString &code) {
    if (code == _sessionCode)
        return;

    _sessionCode = code;

    emit sessinonCodeChanged();
}

const QByteArray &UserModel::sellerToken() const {
    return _sellerToken;
}

void UserModel::setSellerToken(const QByteArray &newSellerToken) {
    _sellerToken = newSellerToken;
    setFSaller(newSellerToken.size());
}

const QString &UserModel::sessionCode() const {
    return _sessionCode;
}

QByteArray UserModel::userKey() const {
    if (!_user)
        return {};
    return _user->getKey();
}

void UserModel::becomeSellerRequest() {
    emit sigBecomeSeller();
}

QString UserModel::userBackUpPath() const {
    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    return path;
}

QString UserModel::userBackUpData() const {
    return _user->secret().toBase64(QByteArray::Base64UrlEncoding);
}

}
