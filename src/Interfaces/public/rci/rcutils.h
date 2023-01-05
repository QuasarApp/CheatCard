
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
     * @brief makeUserId This method make short user id from the userKey.
     * @param userKey This is public user key.
     * @return short user id.
     */
    static unsigned int makeUserId(const QByteArray& userKey);

    /**
     * @brief makeUsersCardsId This method generate userscards id by user id and card id.
     * @param user This is user id
     * @param card This is card id
     * @return id of user data by card.
     */
    static unsigned long long makeUsersCardsId(unsigned int user, unsigned int card);

    /**
     * @brief getUserIdFromUsrsCards This method return an user id from the userscards id
     * @param userscard This is users cards id
     * @return an user id from the userscards id
     */
    static unsigned int getUserIdFromUsrsCards(long long userscard);

    /**
     * @brief getUserIdFromUsrsCards This method return an card id from the userscards id
     * @param userscard This is users cards id
     * @return an card id from the userscards id
     */
    static unsigned int getCardIdFromUsrsCards(long long userscard);

    /**
     * @brief makeUserKey This method create a public user key from user secreet.
     * @param secret This is user secreet key.
     * @return publick yser key.
     */
    static QByteArray makeUserKey(const QByteArray &secret);

};
}
#endif // RCUTILS_H
