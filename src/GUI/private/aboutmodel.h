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

    Q_INVOKABLE QString getCoreLidVr();
    Q_INVOKABLE QString getHearteLibVr();

};

}

#endif // ABOUTMODEL_H
