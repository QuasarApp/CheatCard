//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef DBv0_CARD_H
#define DBv0_CARD_H

#include <rci/objects/icard.h>

#include <dbobject.h>

namespace RC {
namespace DBv0 {

/**
 * @brief The Card class is Database implementation of the iCard interface
 */
class Card: public QH::PKG::DBObject
{
    ONLY_DATABASE_PACKAGE
public:
    Card();

    unsigned int id() const;
    void setId(unsigned int newId);
    QH::PKG::DBObject *createDBObject() const override;
    const QByteArray &logo() const;
    void setLogo(const QByteArray &newImage);
    int getFreeIndex() const;
    void setFreeIndex(int newFreeIndex);
    const QString &getColor() const;
    void setColor(const QString &newColor);
    const QString &getFontColor() const;
    void setFontColor(const QString &newColor);
    const QByteArray &seal() const;
    void setSeal(const QByteArray &newSeal);
    const QString &phone() const;
    void setPhone(const QString &newPhone);
    const QString &telegramm() const;
    void setTelegramm(const QString &newTelegramm) ;
    const QString &instagramm() const ;
    void setInstagramm(const QString &newInstagramm) ;
    const QString &physicalAddress() const ;
    void setPhysicalAddress(const QString &newPhysicalAddress) ;
    const QString &webSite() const ;
    void setWebSite(const QString &newWebSite) ;
    const QString &title() const ;
    void setTitle(const QString &newTitle) ;
    const QByteArray &background() const ;
    void setBackground(const QByteArray &newBackground) ;
    unsigned int cardId() const ;
    const QString &freeItemName() const ;
    void setFreeItemName(const QString &newFreeItemName) ;
    unsigned int getCardVersion() const ;
    void setCardVersion(unsigned int newCardVersion) ;
    void idGen();
    const QByteArray &ownerSignature() const ;
    void setOwnerSignature(const QByteArray &newOwnerSignature) ;

    QString toString() const override;
    bool fromSqlRecord(const QSqlRecord &q) override;
    QString table() const override;
    bool isValid() const override;

    QH::PKG::DBVariantMap variantMap() const override;

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

    QString primaryKey() const override;
    QString primaryValue() const override;

private:

    unsigned int _id = 0;
    QString _title;
    QByteArray _logo;
    QByteArray _seal;
    QByteArray _background;
    QByteArray _ownerSignature;

    QString _phone;
    QString _telegramm;
    QString _instagramm;
    QString _physicalAddress;
    QString _webSite;
    QString _freeItemName;
    unsigned int cardVersion = 0;

    int freeIndex = 1;
    QString color = "#777777";
    QString fontColor = "#000000";

};

}
}

#endif // DB_CARD_H
