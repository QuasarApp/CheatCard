//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef ABOUTMODEL_H
#define ABOUTMODEL_H

#include <QObject>
#include "CheatCardGui/gui_global.h"

namespace RC {

class CHEATCARD_GUI_EXPORT AboutModel: public QObject
{
    Q_OBJECT

public:
    AboutModel();

    Q_INVOKABLE QString getCoreLibVr() const;
    Q_INVOKABLE QString getHeartLibVr() const;
    Q_INVOKABLE QString getZxingCppLibVr() const;
    Q_INVOKABLE QString getSCodesLibVr() const;

};

}

#endif // ABOUTMODEL_H
