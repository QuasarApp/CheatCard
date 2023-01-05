//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef DB_H
#define DB_H

namespace RC {

/**
 * @brief Interfaces namespace is base name space for all interfaces of the cheatcard app.
 */
namespace Interfaces {

/**
 * @brief init This method initialize the local database resources for application.
 * @return true if function finished successfull else false.
 */
bool init();

}
}

#endif // DB_H
