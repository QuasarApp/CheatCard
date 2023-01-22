
//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef RCUTILS_H
#define RCUTILS_H

#include "rci/global.h"

namespace RC {
/**
 * @brief The RCUtils class contais general functions for all application.
 */
class CHEATCARD_INTERFACES_EXPORT RCUtils
{
public:
    RCUtils();
    /**
     * @brief makeUserKey This method create a public user key from user secreet.
     * @param secret This is user secreet key.
     * @return publick yser key.
     */
    static QByteArray makeUserKey(const QByteArray &secret);

    /**
     * @brief randomSHA256 This method create a random sha256 hash.
     * @return random sha256 hash
     */
    static QByteArray randomSHA256();

};
}
#endif // RCUTILS_H
