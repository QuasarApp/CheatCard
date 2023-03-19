
//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef RCUTILS_H
#define RCUTILS_H

#include "rci/global.h"

#include <QSharedPointer>

namespace RC {


namespace Interfaces {
class iUser;
class iContacts;
class iCard;
}

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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    /**
     * @brief makeOlduserId This method create a public old user id from user key.
     * @param secret This is user public key.
     * @return old  user id.
     */
    static unsigned int makeOlduserId(const QByteArray &keys);
#endif

    /**
     * @brief randomSHA256 This method create a random sha256 hash.
     * @return random sha256 hash
     */
    static QByteArray randomSHA256();

    /**
     * @brief convrtOldIdToSHA256 This is static method that convert old unsigned int id to the new sha256 id.
     * @param oldId This is old id value.
     * @return new sha256 id value.
     */
    static QByteArray convrtOldIdToSHA256(unsigned int oldId);

    /**
     * @brief createContact This method fill a empty contact (@a resultContact) object
     * @param baseUser This is user object that will create permision for the @a anotherUser object
     * @param anotherUser This is user that receive access to cards of the @a baseUser object.
     * @param resultContact This is result contact object.
     * @return true if the contact created successful.
     */
    static bool createContact(const QSharedPointer<Interfaces::iUser> &baseUser,
                              const QSharedPointer<Interfaces::iUser> &anotherUser,
                              QSharedPointer<Interfaces::iContacts> &resultContact);

    /**
     * @brief createContact This method fill a empty contact (@a resultContact) object
     * @param baseUser This is user object that will create permision for the @a anotherUser object
     * @param anotherUser This is user that receive access to cards of the @a baseUser object.
     * @param info This is descriptions of the contact.

     * @param resultContact This is result contact object.
     * @return true if the contact created successful.
     */
    static bool createContact(const QByteArray &baseUser,
                              const QByteArray &anotherUser,
                              const QString& info,
                              QSharedPointer<Interfaces::iContacts> &resultContact);

};
}
#endif // RCUTILS_H
