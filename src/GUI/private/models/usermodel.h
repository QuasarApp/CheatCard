//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef USERMODEL_H
#define USERMODEL_H

#include "basemodel.h"

#include <QSharedPointer>
#include <qqml.h>
#include <settingslistner.h>

#include <CheatCardGui/ibilling.h>


namespace RC {
namespace Interfaces {
class iUser;
}

class UserHeader;

class UserModel: public QObject, public QuasarAppUtils::SettingsListner, public BaseModel
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY objChanged)
    Q_PROPERTY(int userId READ userId NOTIFY objChanged)

    Q_PROPERTY(bool fSaller READ fSaller NOTIFY objChanged)
    Q_PROPERTY(long long sessinon READ getSessinon NOTIFY sessinonChanged)
    Q_PROPERTY(QString sessionCode READ sessionCode NOTIFY sessinonCodeChanged)

    QML_ELEMENT

public:
    UserModel(QSharedPointer<Interfaces::iUser> user);

    bool fSaller() const;
    void setFSaller(bool newFSaller);
    QString name() const;
    void setName(const QString &newName);

    QSharedPointer<Interfaces::iUser> user() const;
    void setUser(const QSharedPointer<Interfaces::iUser> &newUser);

    long long getSessinon() const;
    const QString &sessionCode() const;

    Q_INVOKABLE void becomeSellerRequest();
    Q_INVOKABLE QString userBackUpPath() const;

    const QByteArray &sellerToken() const;
    void setSellerToken(const QByteArray &newSellerToken);

    void regenerateSessionKey();

    Q_INVOKABLE QString userBackUpData() const;

    int userId() const;
    UserHeader getHelloPackage() const;

signals:
    void objChanged();
    void sigBecomeSeller();
    void sessinonChanged();

    void sessinonCodeChanged();

protected:
    void setSessinon(long long newSessinon);
    void handleSettingsChanged(const QString& key, const QVariant& value) override;

private:
    void setSessionCode(const QString& code);

    QSharedPointer<Interfaces::iUser> _user;
    long long sessinon;
    QString _sessionCode;
    QByteArray _sellerToken;
};

}
#endif // USERMODEL_H
