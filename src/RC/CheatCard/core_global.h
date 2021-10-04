//#
//# Copyright (C) 2018-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CHEATCARD_CORE_GLOBAL_H
#define CHEATCARD_CORE_GLOBAL_H

#include <QtCore/qglobal.h>


#if defined(CHEATCARD_CORE_LIBRARY)
#  define CHEATCARD_CORE_EXPORT Q_DECL_EXPORT
#else
#  define CHEATCARD_CORE_EXPORT Q_DECL_IMPORT
#endif

#define DEFAULT_CHEAT_CARD_HOST "quasarapp.ddns.net"
#define DEFAULT_CHEAT_CARD_PORT 7122

#define CHEAT_CARD_VERSION "0.."

#endif //CHEATCARD_CORE_GLOBAL_H

