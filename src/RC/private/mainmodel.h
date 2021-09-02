//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QObject>

namespace RC {

/**
 * @brief The MainModel class is main model of the application.
 */
class MainModel : public QObject
{
    Q_OBJECT

public:
    MainModel();
};

}
#endif // MAINMODEL_H
