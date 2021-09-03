//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QObject>
#include "database.h"

namespace RC {

class User;
class Saller;

/**
 * @brief The MainModel class is main model of the application.
 */
class MainModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool fSeller READ getFSeller NOTIFY fSellerChanged)

public:
    MainModel(DB* db);

    bool getFSeller() const;
    Q_INVOKABLE void makeSaller(Saller * saller);

protected:
    void setFSeller(bool newFSeller);

signals:
    void fSellerChanged();

private:
    DB * _db = nullptr;
    User *_currentUser = nullptr;
    bool fSeller = false;
};

}
#endif // MAINMODEL_H
