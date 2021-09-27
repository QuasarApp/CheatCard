//#
//# Copyright (C) 2018-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CHEATCARD_GLOBAL_H
#define CHEATCARD_GLOBAL_H

#include <QtCore/qglobal.h>


#if defined(CHEATCARD_LIBRARY)
#  define CHEATCARD_EXPORT Q_DECL_EXPORT
#else
#  define CHEATCARD_EXPORT Q_DECL_IMPORT
#endif

#define DEFAULT_CHEAT_CARD_HOST "localhost"
#define DEFAULT_CHEAT_CARD_PORT 7122

#endif //CHEATCARD_GLOBAL_H

