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

namespace API {
class User;

}

class UserModel: public QObject, public QuasarAppUtils::SettingsListner
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY objChanged)
    Q_PROPERTY(int userId READ userId NOTIFY objChanged)

    Q_PROPERTY(bool fSaller READ fSaller NOTIFY objChanged)
    Q_PROPERTY(long long sessinon READ getSessinon NOTIFY sessinonChanged)
    Q_PROPERTY(QString sessionCode READ sessionCode NOTIFY sessinonCodeChanged)

    QML_ELEMENT

public:
    UserModel(QSharedPointer<API::User> user);

    bool fSaller() const;
    void setFSaller(bool newFSaller);
    QString name() const;
    void setName(const QString &newName);

    QSharedPointer<API::User> user() const;
    void setUser(const QSharedPointer<API::User> &newUser);

    long long getSessinon() const;
    const QString &sessionCode() const;

    Q_INVOKABLE void becomeSellerRequest();
    Q_INVOKABLE QString userBackUpPath() const;

    const QByteArray &sellerToken() const;
    void setSellerToken(const QByteArray &newSellerToken);

    void regenerateSessionKey();

    Q_INVOKABLE QString userBackUpData() const;

    int userId() const;

signals:
    void objChanged();
    void sigBecomeSeller();
    void sessinonChanged();

    void sessinonCodeChanged();

protected:
    void setSessinon(long long newSessinon);
    API::UserHeader getHelloPackage() const;
    void handleSettingsChanged(const QString& key, const QVariant& value) override;

private:
    void setSessionCode(const QString& code);

    QSharedPointer<API::User> _user;
    long long sessinon;
    QString _sessionCode;
    QByteArray _sellerToken;
};

}
#endif // USERMODEL_H
