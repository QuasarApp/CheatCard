//#
//# Copyright (C) 2018-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef RENAME_ME_GLOBAL_H
#define RENAME_ME_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RENAME_ME_LIBRARY)
#  define RENAME_ME_EXPORT Q_DECL_EXPORT
#else
#  define RENAME_ME_EXPORT Q_DECL_IMPORT
#endif

#endif //RENAME_ME_GLOBAL_H

