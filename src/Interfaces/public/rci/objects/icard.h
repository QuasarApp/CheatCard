//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef ICARD_H
#define ICARD_H


#include "humanreadableobject.h"
#include "rci/global.h"
#include <QSharedPointer>

namespace RC {
namespace Interfaces {

/**
 * @brief The iCard interface sescribe card items.
 */
class CHEATCARD_INTERFACES_EXPORT iCard: public QuasarAppUtils::iHRO
{
public:
    iCard() = default;
    virtual ~iCard() = default;

    /**
     * @brief isValid This method should be return true if this object is correct.
     * @return true if this object is correct else false.
     */
    virtual bool isValid() const = 0;

    /**
     * @brief logo This method should be return rawData of card logo
     * @return rawData of card logo
     */
    virtual const QByteArray &logo() const = 0;

    /**
     * @brief setLogo This method sets new data of card logo.
     * @param newImage This is byte array of logo image.
     */
    virtual void setLogo(const QByteArray &newImage) = 0;

    /**
     * @brief getFreeIndex This method return count items that user should be collect for get bonus.
     * @return count items that user should be collect for get bonus.
     */
    virtual int getFreeIndex() const = 0;

    /**
     * @brief setFreeIndex This method sets new value of the FreeIndex (iCard::getFreeIndex) property.
     * @param newFreeIndex This is new value of the  FreeIndex (iCard::getFreeIndex) property.
     */
    virtual void setFreeIndex(int newFreeIndex) = 0;

    /**
     * @brief getColor This method return main color of the card.
     * @return main color of the card.
     */
    virtual const QString &getColor() const = 0;

    /**
     * @brief setColor This method should set new values for the main color property
     * @param newColor This is new value of the main color.
     */
    virtual void setColor(const QString &newColor) = 0;

    /**
     * @brief getFontColor This method return color of fonts on the card.
     * @return  color of fonts on the card.
     */
    virtual const QString &getFontColor() const = 0;

    /**
     * @brief setFontColor This method sets new value of the fontColor property.
     * @param newColor This is new value of the font color on the card.
     */
    virtual void setFontColor(const QString &newColor) = 0;

    /**
     * @brief seal This is raw bytes arrray of the seal image.
     * @return  raw bytes arrray of the seal image.
     */
    virtual const QByteArray &seal() const = 0;

    /**
     * @brief setSeal This method sets new value of the seal image.
     * @param newSeal this is new value of the seal image.
     */
    virtual void setSeal(const QByteArray &newSeal) = 0;

    /**
     * @brief phone This method return phone number of the card.
     * @return phone number of the card.
     */
    virtual const QString &phone() const = 0;

    /**
     * @brief setPhone This method sets new value of the card's phone.
     * @param newPhone This is new value of the phone.
     */
    virtual void setPhone(const QString &newPhone) = 0;

    /**
     * @brief telegramm This telegramm address of the card.
     * @return telegramm address of the card.
     */
    virtual const QString &telegramm() const = 0;

    /**
     * @brief setTelegramm This method sets new value of telegramm address
     * @param newTelegramm this is new value of the telegramm
     */
    virtual void setTelegramm(const QString &newTelegramm) = 0;

    /**
     * @brief instagramm This instagramm address of the card.
     * @return instagramm address of the card.
     */
    virtual const QString &instagramm() const = 0;

    /**
     * @brief setInstagramm This method sets new value of instagramm address
     * @param new Instagramm this is new value of the telegramm
     */
    virtual void setInstagramm(const QString &newInstagramm) = 0;

    /**
     * @brief physicalAddress This method return physical address of the card.
     * @return  physical address of the card.
     */
    virtual const QString &physicalAddress() const = 0;

    /**
     * @brief setPhysicalAddress This method sets new value of the physical address of the card.
     * @param newPhysicalAddress This is new values of the physical address.
     */
    virtual void setPhysicalAddress(const QString &newPhysicalAddress) = 0;

    /**
     * @brief webSite This methd return url addres of the web site of the card.
     * @return url addres of the web site of the card.
     */
    virtual const QString &webSite() const = 0;

    /**
     * @brief setWebSite This method sets new value of the web url of the card.
     * @param newWebSite This is new value of the web url.
     */
    virtual void setWebSite(const QString &newWebSite) = 0;

    /**
     * @brief title This is title of the card.
     * @return title This is title of the card.
     */
    virtual const QString &title() const = 0;

    /**
     * @brief setTitle this method sets new value of the card's title.
     * @param newTitle This is new value of the title.
     */
    virtual void setTitle(const QString &newTitle) = 0;

    /**
     * @brief background This is byte array of the card background image.
     * @return array of the background image.
     */
    virtual const QByteArray &background() const = 0;

    /**
     * @brief setBackground This method sets new background of the card.
     * @param newBackground This is new valude of the card.
     */
    virtual void setBackground(const QByteArray &newBackground) = 0;

    /**
     * @brief cardKey This is sha256 card id. This id using as a main indentificator of the card item.
     * @return sha256 key of the card.
     */
    virtual const QByteArray &cardKey() const = 0;

    /**
     * @brief setCardKey This method update cards ids (key and shord id).
     * @param newKey This is new value of the card key.
     */
    virtual void setCardKey(const QByteArray& newKey) = 0;

    /**
     * @brief freeItemName This method should be return a bonus name of the card
     * @return  a bonus name of the card
     */
    virtual const QString &freeItemName() const = 0;

    /**
     * @brief setFreeItemName This method sets new value of the bonus name of the card.
     * @param newFreeItemName this is new value of the bonus name.
     */
    virtual void setFreeItemName(const QString &newFreeItemName) = 0;

    /**
     * @brief getCardVersion This method should be return versin of the card item.
     * @return  versin of the card item.
     */
    virtual unsigned int getCardVersion() const = 0;

    /**
     * @brief setCardVersion This method sets new value of the card version.
     * @param newCardVersion This is new value of the card version.
     */
    virtual void setCardVersion(unsigned int newCardVersion) = 0;

    /**
     * @brief ownerSignature This method should be return signature of card ovner.
     * @returnsignature of card ovner.
     */
    virtual const QByteArray &ownerSignature() const = 0;

    /**
     * @brief setOwnerSignature This method sets new signature of the card's ovner.
     * @param newOwnerSignature This is new value of the card ovner.
     */
    virtual void setOwnerSignature(const QByteArray &newOwnerSignature) = 0;

    /**
     * @brief isOvner This method should be check card to owning.
     * @param userId This is id of the checked user.
     * @return true if the @a userId is real card's ovner.
     */
    virtual bool isOvner(const unsigned int userId) const = 0;

    /**
     * @brief compare This method compare currnet object with the @a othe object.
     * @param other This is another card object.
     * @return true if the objects is identical.
     */
    virtual bool compare(const QSharedPointer<Interfaces::iCard>& other) = 0;

};

}
}

Q_DECLARE_METATYPE(QSharedPointer<RC::Interfaces::iCard>)

#endif // ICARD_H
