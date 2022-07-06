//#
//# Copyright (C) 2018-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CHEATCARD_API_GLOBAL_H
#define CHEATCARD_API_GLOBAL_H

#include <QtCore/qglobal.h>


#if defined(CHEATCARD_DB_LIBRARY)
#  define CHEATCARD_DB_EXPORT Q_DECL_EXPORT
#else
#  define CHEATCARD_DB_EXPORT Q_DECL_IMPORT
#endif

#define LAST_RECORD "LAST_RECORDS"

#endif //CHEATCARD_CORE_GLOBAL_H

