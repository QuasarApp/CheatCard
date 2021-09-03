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
    Q_PROPERTY(bool fSeller READ fSeller NOTIFY fSellerChanged)
    Q_PROPERTY(QObject * sallerModel READ sallerModel NOTIFY sallerModelChanged)

public:
    MainModel(DB* db);

    bool fSeller() const;
    Q_INVOKABLE void makeSaller(Saller * saller);

    QObject *sallerModel() const;
    void setSallerModel(Saller *newSallerModel);

signals:
    void fSellerChanged();
    void sallerModelChanged();

private:
    DB * _db = nullptr;
    User *_currentUser = nullptr;
    Saller *_sallerModel = nullptr;

};

}
#endif // MAINMODEL_H
