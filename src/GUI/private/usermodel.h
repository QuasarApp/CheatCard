//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef USERMODEL_H
#define USERMODEL_H

#include <QSharedPointer>
#include <qqml.h>

#include <CheatCard/userheader.h>


namespace RC {

class User;

class UserModel: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY objChanged)

    Q_PROPERTY(bool fSaller READ fSaller WRITE setFSaller NOTIFY objChanged)
    Q_PROPERTY(long long sessinon READ getSessinon NOTIFY sessinonChanged)
    Q_PROPERTY(QString sessionCode READ sessionCode NOTIFY sessinonChanged)

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

signals:
    void objChanged();

    void sessinonChanged();

protected:
    void setSessinon(long long newSessinon);
    UserHeader getHelloPackage() const;

private:

    QSharedPointer<User> _user;
    long long sessinon;
    QString _sessionCode;
};

}
#endif // USERMODEL_H
