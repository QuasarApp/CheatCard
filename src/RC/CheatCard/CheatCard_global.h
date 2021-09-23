//#
//# Copyright (C) 2018-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CheatCard_GLOBAL_H
#define CheatCard_GLOBAL_H

#include <QtCore/qglobal.h>


#if defined(CheatCard_LIBRARY)
#  define CheatCard_EXPORT Q_DECL_EXPORT
#else
#  define CheatCard_EXPORT Q_DECL_IMPORT
#endif

enum Commands {
    Undefined = 0,
    UserId = 1,
    StatusRequest,
    StatusResponce,
    CardData,

    CardDataRequest,

    Successful
};

#ifdef QT_DEBUG
#define RC_WAIT_TIME 50000
#else
#define RC_WAIT_TIME 5000
#endif

#endif //CheatCard_GLOBAL_H

