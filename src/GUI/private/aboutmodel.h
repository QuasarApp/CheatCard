//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef ABOUTMODEL_H
#define ABOUTMODEL_H

#include <QObject>

namespace RC {

class AboutModel: public QObject
{
    Q_OBJECT

public:
    AboutModel();

    Q_INVOKABLE QString getCoreLibVr();
    Q_INVOKABLE QString getHeartLibVr();
    Q_INVOKABLE QString getZxingCppLibVr();
    Q_INVOKABLE QString getSCodesLibVr();

};

}

#endif // ABOUTMODEL_H
