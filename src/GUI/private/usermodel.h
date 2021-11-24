//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef USERMODEL_H
#define USERMODEL_H

#include <QSharedPointer>
#include <qqml.h>
#include <settingslistner.h>

#include <CheatCard/api/api0/userheader.h>

#include <CheatCardGui/ibilling.h>


namespace RC {

class User;

class UserModel: public QObject, public QuasarAppUtils::SettingsListner
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY objChanged)

    Q_PROPERTY(bool fSaller READ fSaller NOTIFY objChanged)
    Q_PROPERTY(long long sessinon READ getSessinon NOTIFY sessinonChanged)
    Q_PROPERTY(QString sessionCode READ sessionCode NOTIFY sessinonChanged)
    Q_PROPERTY(QString userBackUp READ userBackUp NOTIFY objChanged)

    QML_ELEMENT

public:
    UserModel(QSharedPointer<User> user);

    bool fSaller() const;
    void setFSaller(bool newFSaller);
    QString name() const;
    void setName(const QString &newName);

    QSharedPointer<User> user() const;
    void setUser(const QSharedPointer<User> &newUser);

    long long getSessinon() const;
    const QString &sessionCode() const;

    Q_INVOKABLE void becomeSellerRequest() const;
    Q_INVOKABLE QString userBackUpPath() const;

    const QByteArray &sellerToken() const;
    void setSellerToken(const QByteArray &newSellerToken);

    void regenerateSessionKey();

    const QString &userBackUp() const;

signals:
    void objChanged();
    void sigBecomeSeller() const;
    void sessinonChanged();

protected:
    void setSessinon(long long newSessinon);
    UserHeader getHelloPackage() const;
    void handleSettingsChanged(const QString& key, const QVariant& value) override;

private:

    QSharedPointer<User> _user;
    long long sessinon;
    QString _sessionCode;
    QByteArray _sellerToken;
    QString _userBackUp;
};

}
#endif // USERMODEL_H
