//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef IDB_H
#define IDB_H

#include <QSharedPointer>
#include <rci/objects/icard.h>
#include <rci/objects/icontacts.h>
#include <rci/objects/iuser.h>
#include <rci/objects/iuserscards.h>
#include "rci/global.h"
#include "rci/objects/isession.h"

namespace DP {
class iPill;
}
namespace RC {

namespace Interfaces {

const int ACTIVE_USER_TIME_LIMIT = 60 * 60 * 24 * 7;// 1 weack

/**
 * @brief The IDB class Contains functions for gettings objects from database.
 * The IDB class has implementation for each api versions.
 */
class CHEATCARD_INTERFACES_EXPORT iDB
{
public:

    /**
     * @brief init This is wrapper of the init database methods.
     * @return true if database initialized successful else false.
     */
    virtual bool init() = 0;

    /**
     * @brief isActive This method check database status.
     * @return true if the database is active (initialized successful and can send queries to the database driver) else false.
     */
    virtual bool isInit() const = 0;

    /**
     * @brief deleteContact This method remove contact object from database.
     * @param contact This is contact object that will be removeed
     * @return true if session removed successful.
     */
    virtual bool deleteContact(const QSharedPointer<iContacts>& contact) const = 0;

    /**
     * @brief deleteSessuon This method remove the session object from database.
     * @param sessionId This is id of session
     * @return true if session removed successful.
     */
    virtual bool deleteSessuon(long long sessionId) const = 0;

    /**
     * @brief deleteCard This method remove the card and card data.
     * @param cardId This is id of card
     * @return true if session removed successful.
     */
    virtual bool deleteCard(unsigned int cardId) const = 0;

    /**
     * @brief deleteContactsByChildUserKey This method delete contacts with selected childUser.
     * @param childUser This is id of the child user.
     * @return true if contact removed successful.
     */
    virtual bool deleteContactsByChildUserKey(const QByteArray& childUser) const = 0;

    /**
     * @brief makeEmptyContact This method create empty contact object
     * @return empty contact object
     */
    virtual QSharedPointer<iContacts> makeEmptyContact() const = 0;

    /**
     * @brief makeEmptyUser This method create empty user object
     * @return  empty user object
     */
    virtual QSharedPointer<iUser> makeEmptyUser() const = 0;

    /**
     * @brief makeEmptyUser This method create empty card object
     * @return  empty card object
     */
    virtual QSharedPointer<iCard> makeEmptyCard() const = 0;

    /**
     * @brief makeEmptyUser This method create empty users data object
     * @return  empty users data object
     */
    virtual QSharedPointer<iUsersCards> makeEmptyUsersCard() const = 0;

    /**
     * @brief makeEmptyUser This method create session data object
     * @return  empty session object
     */
    virtual QSharedPointer<iSession> makeEmptySession() const = 0;

    /**
     * @brief saveUser This method save user object into database.
     * @param user This is user object
     * @return true if saving finsihed successful else false
     */
    virtual bool saveUser(const QSharedPointer<iUser>& user) const = 0;

    /**
     * @brief saveCard This method save card object into database.
     * @param card This is card object
     * @return true if saving finsihed successful else false
     */
    virtual bool saveCard(const QSharedPointer<iCard>& card) const = 0;

    /**
     * @brief saveUsersCard This method save user data into database.
     * @param userData This is users data object.
     * @return true if saving finsihed successful else false
     */
    virtual bool saveUsersCard(const QSharedPointer<iUsersCards>& userData) const = 0;

    /**
     * @brief saveSession This method save session object into database.
     * @param session This is session object.
     * @return true if saving finsihed successful else false
     */
    virtual bool saveSession(const QSharedPointer<iSession>& session) const = 0;

    /**
     * @brief saveContact This method save contact object into database.
     * @param contact This is caontact object.
     * @return true if saving finsihed successful else false
     */
    virtual bool saveContact(const QSharedPointer<iContacts>& contact) const = 0;

    /**
     * @brief getCardVersion This method return version of the card in database.
     * @param cardId This is card id
     * @return version of the card in database.
     */
    virtual unsigned int getCardVersion(unsigned int cardId) const = 0;

    /**
     * @brief getCardField This method return any field from the card data.
     * @param cardId This is card id
     * @param field This is needed filed of the card.
     * @return card's field value.
     */
    virtual QVariant getCardField(unsigned int cardId, const QString& field) = 0;

    /**
     * @brief getUsersCardsFromSession This method return list of users data from session object.
     * @param sessionId This is id of the nedded session.
     * @return list of users data from session object.
     */
    virtual QList<QSharedPointer<iUsersCards>>
    getUsersCardsFromSession(long long sessionId) = 0;

    /**
     * @brief getUser This method return object of use from db
     * @param userId This is use id
     * @return object of use from db
     */
    virtual QSharedPointer<iUser>
    getUser(unsigned int userId) const = 0;

    /**
     * @brief getAllUserData This method return all user's data from db
     * @param userId this is user id.
     * @return all user's data from db
     */
    virtual QList<QSharedPointer<iUsersCards>>
    getAllUserData(unsigned int userId) const = 0;

    /**
     * @brief getUserCardData This method return shared data between user with @a userId and card with @a cardId
     * @param userId This is id of checked user
     * @param cardId This is id of checked card item
     * @return shared data between user with @a userId and card with @a cardId
     */
    virtual QSharedPointer<iUsersCards>
    getUserCardData(unsigned int userId,
                    unsigned int cardId) const = 0;

    /**
     * @brief getAllUserFromCard This method return all data of the card @a cardId.
     * @param cardId This is id of checked card.
     * @param ignoreUserId this is id of ignored user.
     * @note The @a ignoreUserId argument can be used for fetch all data for owner. because owner set one seal itself.
     * @return  all data of the card @a cardId.
     */
    virtual QList<QSharedPointer<iUsersCards>>
    getAllUserFromCard(unsigned int cardId,
                       unsigned int ignoreUserId = 0) const = 0;

    /**
     * @brief getAllActiveUserFromCard This method return all a data of all active users on the card @a cardId.
     * @param cardId This is id of checked card.
     * @param unixTimeRange This is time range for active users.
     * @param ignoreUserId this is id of ignored user.
     * @return all data of active users of the card @a cardId.
     * @see IDB::getAllPassiveUserFromCard
     */
    virtual QList<QSharedPointer<iUsersCards>>
    getAllActiveUserFromCard(unsigned int cardId, int unixTimeRange,
                             unsigned int ignoreUserId = 0) const = 0;

    /**
     * @brief getAllPassiveUserFromCard This method return all a data of all passive users on the card @a cardId.
     * @param cardId This is id of checked card.
     * @param unixTimeRange This is time range for passive users.
     * @param ignoreUserId this is id of ignored user.
     * @return all data of passive users of the card @a cardId.
     * @see IDB::getAllActiveUserFromCard
     */
    virtual QList<QSharedPointer<iUsersCards>>
    getAllPassiveUserFromCard(unsigned int cardId, int unixTimeRange,
                              unsigned int ignoreUserId = 0) const = 0;

    /**
     * @brief getAllUserDataFromCard This method return list of users that use card @a cardId
     * @param cardId This is card id
     * @return list of users that use card @a cardId
     */
    virtual QList<QSharedPointer<iUser>>
    getAllUserDataFromCard(unsigned int cardId) const = 0;

    /**
     * @brief getAllUserWithPrivateKeys This method return list of users that has private keys. (not guest users records)
     * @return  list of users that has private keys.
     */
    virtual QList<QSharedPointer<iUser>>
    getAllUserWithPrivateKeys() const = 0;

    /**
     * @brief getCard This method return card by id.
     * @param cardId This is card id.
     * @return card by id.
     */
    virtual QSharedPointer<iCard>
    getCard(unsigned int cardId) = 0;

    /**
     * @brief getAllUserCards This method will return list of available cards of the user with @a userKey key
     * @param userKey user key
     * @param restOf This option force return list of not ovned cards.
     * @return cards list;
     */
    virtual QList<QSharedPointer<iCard>>
    getAllUserCards(const QByteArray &userKey,
                    bool restOf = false,
                    const QList<QSharedPointer<iContacts>>& childs = {}) = 0;
    /**
     * @brief getAllUserCardsData This method will return list of available userscards data of the user with @a userKey key
     * @param userKey user key
     * @return cards data list;
     */
    virtual QList<QSharedPointer<iUsersCards>>
    getAllUserCardsData(const QByteArray &userKey,
                        const QList<QSharedPointer<iContacts>>& childs = {}) = 0;

    /**
     * @brief getContactFromChildId This method return contact by child users id
     * @param userKey This is key of main user
     * @param childUserId this is id of child user.
     * @return contact by child users id
     */
    virtual QSharedPointer<iContacts>
    getContactFromChildId(const QByteArray& userKey, const QByteArray& childUserId) = 0;

    /**
     * @brief getMasterKeys This method return list of master contacts of the child user.
     * @param childUserId This is id of child user.
     * @return list of master contacts of the child user.
     */
    virtual QList<QSharedPointer<iContacts>>
    getMasterKeys(const QByteArray& childUserId) = 0;

    /**
     * @brief getSlaveKeys This method return list of slaves contacts of the main user.
     * @param userId This is id of main user.
     * @return list of slaves contacts of the child user.
     */
    virtual QList<QSharedPointer<iContacts>>
    getSlaveKeys(const QByteArray& userId) = 0;

    /**
     * @brief getSecret This method return scret key by the user key
     * @param userKey This is public user key
     * @return secret key by the user key
     */
    virtual QByteArray getSecret(const QByteArray& userKey) const = 0;

    /**
     * @brief getSecret This method return scret key by the user key
     * @param userKey This is public user key
     * @return secret key by the user key
     */
    virtual QByteArray getSecretOfCardOvner(unsigned int cardId) const = 0;

    /**
     * @brief getFreeItemsCount This method return coun of free items by user data
     * @param inputData This is user data.
     * @return free items count
     */
    virtual int getFreeItemsCount(const QSharedPointer<Interfaces::iUsersCards> &inputData) const = 0;

    /**
     * @brief getCountOfReceivedItems This method return count of the receivet bonuses.
     * @param userId This is user id.
     * @param cardId This is card id.
     * @return count of the received bonuses of the user with @a userId in a card with @a cardId
     */
    virtual int getCountOfReceivedItems(unsigned int userId,
                                        unsigned int cardId) = 0;

    /**
     * @brief clearOldData This method remove all date that oldly of the duration time.
     * @param duration This is life duration of the objects.
     * @return true if all objecre removed successful.
     */
    virtual bool clearOldData(int duration) = 0;

    /**
     * @brief initPills This method return requered pill of the database.
     * @param pillId This is required piil id.
     * @return list available pills of the database.
     */
    virtual QSharedPointer<DP::iPill> initPills(const QString& piilId) = 0;

    /**
     * @brief initPills This method return list available pills of the database.
     * @return list available pills of the database.
     * @note This method return all available pills.
     */
    virtual QList<QSharedPointer<DP::iPill>> initPills() = 0;

    /**
     * @brief backUp This method make backup of the database.
     * @param backUpPath This is backup path.
     * If this argument sets to empty value then database will backup to the daful backup folder.
     * @return path to the created backup file. if backup is failed the return empty string..
     * @see AppDataBase::AppDataBase
     * @see AppDataBase::setBackUpPath
     */
    virtual QString backUp(const QString& backUpPath = "") const= 0;
};


}

}
#endif // IDB_H
