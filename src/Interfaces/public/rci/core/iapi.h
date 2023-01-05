//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef IAPI_H
#define IAPI_H

#include "rci/global.h"
#include "rci/objects/icard.h"
#include "rci/objects/isession.h"
#include "rci/objects/icontacts.h"
#include "rci/objects/iuserscards.h"

#include <QSharedPointer>

#define USERREQUEST_TIMEOUT 3

namespace QH {
class AbstractNodeInfo;
}

namespace RC {
namespace Interfaces {

/**
 * @brief The iAPI class is main api interface of the app.
 */
class CHEATCARD_INTERFACES_EXPORT iAPI
{
public:
    iAPI() = default;
    virtual ~iAPI() = default;

    /**
     * @brief sendCardStatusRequest This method should be send request to get status of my account
     * @param userSession this is session id.
     * @param dist This is distanation node.
     */
    virtual void sendCardStatusRequest(long long userSession,
                                       QH::AbstractNodeInfo *dist) = 0;

    /**
     * @brief sendSessions This method send session to server
     * @param sessionsthis is session data.
     * @param dist This is distanation node.
     */
    virtual void sendSessions(const QHash<long long, QSharedPointer<Interfaces::iSession> > &sessions,
                              QH::AbstractNodeInfo *dist) = 0;

    /**
     * @brief restoreOldDateRequest This is request for recavery users data froma server backup
     * @param curentUserKey this is user key
     * @param dist This is distanation node.
     */
    virtual void restoreOldDateRequest(const QByteArray &curentUserKey, QH::AbstractNodeInfo *dist) = 0;

    /**
     * @brief restoreOneCardRequest This is request to restore one card data.
     * @param cardId this is card id that will be restored.
     * @param dist This is distanation node.
     */
    virtual void restoreOneCardRequest(unsigned int cardId, QH::AbstractNodeInfo *dist) = 0;

    /**
     * @brief sendContacts This method send to server new information about contacts
     * @param conntact Tih is contact data
     * @param secreet This is secret of user that want to create contact (autontiification)
     * @param removeRequest This is boolean option for remove old contact data.
     * @param dist This is distanation node.
     * @return true if request created and sent successful else false.
     */
    virtual bool sendContacts(const iContacts& conntact,
                       const QByteArray& secreet,
                       bool removeRequest,
                       QH::AbstractNodeInfo *dist) = 0;

    /**
     * @brief deleteCard This is delete card request
     * @param cardId card id
     * @param curentUserKey This is key of user that want to remove the card with @a cardId
     * @param dist This is distanation node.
     * @return true if request created and sent successful else false.
     */
    virtual bool deleteCard(unsigned int cardId,
                            const QByteArray &curentUserKey,
                            QH::AbstractNodeInfo *dist) = 0;

    /**
     * @brief sendUpdateCard This request send new updated information about the card.
     * @param cardId card id
     * @param version verson of update (should be incremented for apply changes)
     * @param dist This is distanation node.
     * @return true if request created and sent successful else false.
     */
    virtual bool sendUpdateCard(unsigned int cardId, unsigned int version, QH::AbstractNodeInfo *dist) = 0;

    /**
     * @brief changeUsersData This request send new updated information about users purcheses.
     * @param sellerUserKey This is a public key of the user that make a seal on the card.
     * This user must have a access to card.
     * @param cardId card id
     * @param userId user id
     * @param session is session key.
     * @param purchasesCount this is cout of purshise of user
     * @param receivedCount this is cout of received bonuses of user on this card with @a cardId.
     * @param dist This is distanation node.
     * @return true if request created and sent successful else false.
     */
    virtual bool changeUsersData(const QByteArray& sellerUserKey,
                                 unsigned int cardId,
                                 unsigned int userId,
                                 unsigned long long session,
                                 unsigned int purchasesCount,
                                 unsigned int receivedCount,
                                 QH::AbstractNodeInfo *dist) = 0;

signals:
    /**
     * @brief sigPurchaseWasSuccessful This signal will emit when user receive update.
     * @param userData This is new user data.
     * @param alert this is alert mode. If this argument will set to true then application show notification about changes.
     */
    virtual void sigPurchaseWasSuccessful(QSharedPointer<RC::Interfaces::iUsersCards> userData, bool alert) = 0;

    /**
     * @brief sigCardReceived This signal will emit when client receive card information.
     */
    virtual void sigCardReceived(QSharedPointer<RC::Interfaces::iCard>) = 0;

    /**
     * @brief sigSessionStatusResult This signal will emit when client receive responce of changes from server
     * @param succesed This is result.
     */
    virtual void sigSessionStatusResult(QSharedPointer<RC::Interfaces::iSession>, bool succesed) = 0;

    /**
     * @brief sigContactsStatusResult This signal will emit when a client receive responce from server about contact.
     * @param contact This is result of contact object.
     * @param succesed this is result
     * @param removed This will be true when a contact was removed.
     */
    virtual void sigContactsStatusResult(QSharedPointer<RC::Interfaces::iContacts> contact, bool succesed, bool removed) = 0;

    /**
     * @brief sigContactsListChanged This signal will emit when contacts list was changed.
     */
    virtual void sigContactsListChanged() = 0;
};

}
}
#endif // IAPI_H
