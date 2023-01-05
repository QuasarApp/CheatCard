//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef IUSER_H
#define IUSER_H

#include "humanreadableobject.h"
#include "rci/global.h"
#include <QSharedPointer>

namespace RC {
namespace Interfaces {

/**
 * @brief The iUser class This is main interface class for contatins all information about user
 * The user structure is:
 * Secreet - value with that user will be prove to the server that it is owner.
 * User Key - this is hash of secret value (unique user key)
 * user id - short user id using for database key.
 *
 * ## Generate seteps:
 *
 * make secret
 * from secrete make key
 * from key make id.
 */
class CHEATCARD_INTERFACES_EXPORT iUser : public QuasarAppUtils::iHRO
{

public:
    iUser() = default;;
    virtual ~iUser() = default;

    /**
     * @brief isValid This method should be return true if this object is correct.
     * @return true if this object is correct else false.
     */
    virtual bool isValid() const = 0;

    /**
     * @brief id This method should be return user id.
     * @return user id
     */
    virtual unsigned int id() const = 0;

    /**
     * @brief setId This method sets new id for user.
     * @param newId This is new user id
     */
    virtual void setId(unsigned int newId) = 0;

    /**
     * @brief name This method should be return a curent user name
     * @return current user name
     */
    virtual const QString &name() const = 0;

    /**
     * @brief setName This method should be set new user name
     * @param newName this is new value of the user.
     */
    virtual void setName(const QString &newName) = 0;

    /**
     * @brief fSaller This method should return true if the curent user is seller else false.
     * @return  true if the curent user is seller else false.
     */
    virtual bool fSaller() const = 0;

    /**
     * @brief setFSaller This method should be change user seller state to the @a newFSaller value.
     * @param newFSaller This is new seller status.
     */
    virtual void setFSaller(bool newFSaller) = 0;

    /**
     * @brief getKey This method should be return current user's publlic key (user long id)
     * @return current user's public key (user long id)
     */
    virtual const QByteArray &getKey() const = 0;

    /**
     * @brief setKey This method should be set new value for the user key.
     * @param newKey this is new value of an user key
     */
    virtual void setKey(const QByteArray &newKey) = 0;

    /**
     * @brief getSignature This method should be return user's signature. The signatur is base64 implementation of the user key.
     * @return  user's signature
     * @see iUser::getKey
     */
    virtual const QString getSignature() const = 0;

    /**
     * @brief secret This method should be return user secret. The user secret is sha256 byte array
     * @return user secret
     */
    virtual const QByteArray &secret() const = 0;

    /**
     * @brief setSecret This method sets new value for the secret property.
     * @param newSecret This is new value of the secret
     */
    virtual void setSecret(const QByteArray &newSecret) = 0;

    /**
     * @brief regenerateKeys This method regenerate all keys for user.
     * @warning This method may broke old user data... Invoke only when you create new user.
     */
    virtual void regenerateKeys() = 0;

    /**
     * @brief isAllKeysIsValid this method should check all keys to valid
     * @return true if all keys of user is valid else false.
     */
    virtual bool isAllKeysIsValid() const = 0;
};

}
}

Q_DECLARE_METATYPE(QSharedPointer<RC::Interfaces::iUser>)

#endif // IUSER_H
