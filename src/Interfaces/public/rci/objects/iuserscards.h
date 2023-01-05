//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef IUSERSCARDS_H
#define IUSERSCARDS_H

#include "humanreadableobject.h"
#include "rci/global.h"
#include <QSharedPointer>

namespace RC {
namespace Interfaces {

/**
 * @brief The iUsersCards class is main interface that contains description of user's data. For exmaple seals and purchase.
 * @note All data stored as a by suerid and catdid keys.
 */
class CHEATCARD_INTERFACES_EXPORT iUsersCards: public QuasarAppUtils::iHRO
{

public:
    iUsersCards() = default;
    virtual ~iUsersCards() = default;

    /**
     * @brief isValid This method should be return true if this object is correct.
     * @return true if this object is correct else false.
     */
    virtual bool isValid() const = 0;

    /**
     * @brief getUser This method should be rerutn user id.
     * @return user id
     */
    virtual unsigned int getUser() const = 0;

    /**
     * @brief setUser This method should be set new value for the user id property.
     * @param newUser this is new value of the user id.
     */
    virtual void setUser(unsigned int newUser) = 0;

    /**
     * @brief getUser This method should be rerutn catd id.
     * @return card id
     */
    virtual unsigned int getCard() const = 0;

    /**
     * @brief setUser This method should be set new value for the user id property.
     * @param newUser this is new value of the user id.
     */
    virtual void setCard(unsigned int newCard) = 0;

    /**
     * @brief getPurchasesNumber This method sgould be return count of the user's purchases on the card.
     * @return count of purchase of user on the card
     */
    virtual unsigned int getPurchasesNumber() const = 0;

    /**
     * @brief setPurchasesNumber This method sets new count of purhases of an user on a card.
     * @param newPurchasesNumber this is new value of purhases count.
     */
    virtual void setPurchasesNumber(unsigned int newPurchasesNumber) = 0;

    /**
     * @brief getReceived This method should be return count of the received bonuses.
     * @return count of the received bonuses.
     */
    virtual unsigned int getReceived() const = 0;

    /**
     * @brief setReceived This method sets new value of the received bonuses.
     * @param newReceived This is new value of received bonuses.
     */
    virtual void setReceived(unsigned int newReceived) = 0;

    /**
     * @brief receive This method shuuld be update of receive bonusese of user on this card. this method increment received bonuses.
     * @param receiveFreItemsCount value of increment
     */
    virtual void receive(unsigned int receiveFreItemsCount) = 0;

    /**
     * @brief getId This method should be return id of this pair. usualy it is userId + cardId
     * @return long this data item id.
     */
    virtual long long getId() const = 0;

    /**
     * @brief getCardVersion This method should be return card version.
     * @return card version.
     */
    virtual unsigned int getCardVersion() const = 0;

    /**
     * @brief setCardVersion This method sets new card version
     * @param newCardVersion new value of the card version.
     */
    virtual void setCardVersion(unsigned int newCardVersion) = 0;

    /**
     * @brief getRawTime This method return time of last transaction in Unix time.
     * @return time of last transaction in Unix time.
     */
    virtual unsigned int getRawTime() const = 0;

    /**
     * @brief isActive This method check last transaction time and comapre with active range time. if The user is acrive return true else false.
     * @return  if The user is acrive return true else false.
     */
    virtual bool isActive() const = 0;

    /**
     * @brief setTime This method sets new transaction tiem.
     * @param newTime This is new time value
     */
    virtual void setTime(unsigned int newTime) = 0;

};

}
}

Q_DECLARE_METATYPE(QSharedPointer<RC::Interfaces::iUsersCards>)

#endif // IUSERSCARDS_H
