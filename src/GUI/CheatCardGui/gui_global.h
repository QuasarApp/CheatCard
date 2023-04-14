//#
//# Copyright (C) 2018-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CHEATCARD_GUI_GLOBAL_H
#define CHEATCARD_GUI_GLOBAL_H

#include <QtCore/qglobal.h>


#if defined(CHEATCARD_GUI_LIBRARY)
#  define CHEATCARD_GUI_EXPORT Q_DECL_EXPORT
#else
#  define CHEATCARD_GUI_EXPORT Q_DECL_IMPORT
#endif

#endif //CHEATCARD_GUI_GLOBAL_H

