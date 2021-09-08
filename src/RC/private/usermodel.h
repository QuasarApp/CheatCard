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

namespace RC {

class User;

class UserModel: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY objChanged)
    Q_PROPERTY(QString visibleName READ visibleName WRITE setVisibleName NOTIFY objChanged)

    Q_PROPERTY(bool fSaller READ fSaller WRITE setFSaller NOTIFY objChanged)

    QML_ELEMENT

public:
    UserModel(QSharedPointer<User> user);

    bool fSaller() const;
    void setFSaller(bool newFSaller);
    QString name() const;
    void setName(const QString &newName);

    QString visibleName() const;
    void setVisibleName(const QString &newName);

    QSharedPointer<User> user() const;
    void setUser(const QSharedPointer<User> &newUser);

signals:
    void objChanged();

private:
    QSharedPointer<User> _user;
};

}
#endif // USERMODEL_H
