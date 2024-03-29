//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef ABOUTMODEL_H
#define ABOUTMODEL_H

#include <QObject>
#include "CheatCardGui/gui_global.h"
#include "basemodel.h"

namespace RC {

class CHEATCARD_GUI_EXPORT AboutModel: public QObject, public BaseModel
{
    Q_OBJECT

public:
    AboutModel();

    Q_INVOKABLE QString getCoreLibVr() const;
    Q_INVOKABLE QString getHeartLibVr() const;
    Q_INVOKABLE QString getZxingCppLibVr() const;
    Q_INVOKABLE QString getSCodesLibVr() const;
    Q_INVOKABLE QString getQtLibVr() const;
    Q_INVOKABLE QString getSimpleQmlNotifyLibVr() const;
    Q_INVOKABLE QString getDoctorPillLibVr() const;

};

}

#endif // ABOUTMODEL_H
