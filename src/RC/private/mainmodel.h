//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef RC_MAINMODEL_H
#define RC_MAINMODEL_H

#include <QObject>
#include "database.h"

namespace RC {

class User;
class Config;

/**
 * @brief The MainModel class is main model of the application.
 */
class MainModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool fFirst READ fFirst NOTIFY fFirstChanged)
    Q_PROPERTY(QObject * currentUser READ currentUser NOTIFY currentUserChanged)

public:
    MainModel(DB* db);

    bool fFirst() const;

    QObject *currentUser() const;
    void setCurrentUser(User *newCurrentUser);

signals:

    void fFirstChanged();

    void currentUserChanged();

private:
    DB * _db = nullptr;
    QSharedPointer<User> _currentUser;
    QSharedPointer<Config> _config;
};

}
#endif // RC_MAINMODEL_H
