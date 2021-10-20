//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef JAVAPROVIDER_H
#define JAVAPROVIDER_H

#include <QtGlobal>

#ifdef Q_OS_ANDROID
class JavaProvider
{
public:
    JavaProvider();
    void getPremium() const;


};

#endif
#endif // JAVAPROVIDER_H
