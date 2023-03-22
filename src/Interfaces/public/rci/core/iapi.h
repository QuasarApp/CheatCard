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
     * @brief syncRequest This is request for recavery users data froma server backup
     * @param curentUserKey this is user key
     * @param dist This is distanation node.
     */
    virtual void syncRequest(const QByteArray &curentUserKey,
                             QH::AbstractNodeInfo *dist,
                             const std::function<void(bool successful)>& = {}) = 0;

    /**
     * @brief sendContacts This method send to server new information about contacts
     * @param conntact Tih is contact data
     * @param secreet This is secret of user that want to create contact (autontiification)
     * @param removeRequest This is boolean option for remove old contact data.
     * @param dist This is distanation node.
     * @return true if request created and sent successful else false.
     */
    virtual bool sendContacts(const QSharedPointer<Interfaces::iContacts>& conntact,
                              const QByteArray& secreet,
                              bool removeRequest,
                              QH::AbstractNodeInfo *dist,
                              const std::function<void(int err)>& = {}) = 0;

    /**
     * @brief deleteCard This is delete card request
     * @param cardId card id
     * @param curentUserKey This is key of user that want to remove the card with @a cardId
     * @param dist This is distanation node.
     * @return true if request created and sent successful else false.
     */
    virtual bool deleteCard(const QByteArray &cardId,
                            const QByteArray &curentUserKey,
                            QH::AbstractNodeInfo *dist,
                            const std::function<void(int err)>& = {}) = 0;


    /**
     * @brief requestCard This is get card request
     * @param cardId card id
     * @param dist This is distanation node.
     * @return true if request created and sent successful else false.
     */
    virtual bool requestCard(const QByteArray &cardId,
                             QH::AbstractNodeInfo *dist,
                             const std::function<void(int err)>& = {}) = 0;

    /**
     * @brief sendUpdateCard This request send new updated information about the card.
     * @param cardId card id
     * @param version verson of update (should be incremented for apply changes)
     * @param dist This is distanation node.
     * @return true if request created and sent successful else false.
     */
    virtual bool sendUpdateCard(const QByteArray& cardId,
                                unsigned int version,
                                QH::AbstractNodeInfo *dist,
                                const std::function<void(int err)>& = {}) = 0;

    /**
     * @brief changeUsersData This request send new updated information about users purcheses.
     * @param sellerUserKey This is a public key of the user that make a seal on the card.
     * This user must have a access to card.
     * @param cardId card id
     * @param userId user id
     * @param purchasesCount this is cout of purshise of user
     * @param receivedCount this is cout of received bonuses of user on this card with @a cardId.
     * @param dist This is distanation node.
     * @return true if request created and sent successful else false.
     */
    virtual bool changeUsersData(const QByteArray& sellerUserKey,
                                 const QByteArray& cardId,
                                 const QByteArray& userId,
                                 unsigned int purchasesCount,
                                 unsigned int receivedCount,
                                 QH::AbstractNodeInfo *dist,
                                 const std::function<void(int err)>& = {}) = 0;

    /**
     * @brief sendOldVersionPackage This method used for sending update message to the old apps
     * @param dist This is distanation node.
     */
    virtual void sendOldVersionPackage(QH::AbstractNodeInfo *dist) = 0;

signals:
    /**
     * @brief sigPurchaseWasSuccessful This signal will emit when user receive update.
     * @param userData This is new user data.
     * @param alert this is alert mode. If this argument will set to true then application show notification about changes.
     */
    virtual void sigPurchaseWasSuccessful(QSharedPointer<RC::Interfaces::iUsersCards> userData, bool alert) = 0;

    /**
     * @brief sigCardReceived This signal will emit when client receive card information.
     * @param card This is card object.
     */
    virtual void sigCardReceived(QSharedPointer<RC::Interfaces::iCard> card) = 0;

    /**
     * @brief sigCardRemoved This signal will be invoked when usersData will be removed.
     * @param cardId This is sha256 card id.
     * @param userId This is sha256 user id.
     */
    virtual void sigUserDataRemoved(const QByteArray& cardId, const QByteArray& userId) = 0;

    /**
     * @brief sigContactsListChanged This signal will emit when contacts list was changed.
     */
    virtual void sigContactsListChanged() = 0;
};

}
}
#endif // IAPI_H
