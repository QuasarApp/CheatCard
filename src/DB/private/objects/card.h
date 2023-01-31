//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef DB_CARD_H
#define DB_CARD_H

#include <rci/objects/icard.h>

#include <dbobject.h>

namespace RC {
namespace DB {

/**
 * @brief The Card class is Database implementation of the iCard interface
 */
class Card: public RC::Interfaces::iCard, public QH::PKG::DBObject
{
    ONLY_DATABASE_PACKAGE
public:
    Card();
    Card(const QSharedPointer<Interfaces::iCard> &obj);

    const QByteArray& cardId() const override;
    void setCardId(const QByteArray& newId) override;
    QH::PKG::DBObject *createDBObject() const override;
    const QByteArray &logo() const override;
    void setLogo(const QByteArray &newImage) override;
    int getFreeIndex() const override;
    void setFreeIndex(int newFreeIndex) override;
    const QString &getColor() const override;
    void setColor(const QString &newColor) override;
    const QString &getFontColor() const override;
    void setFontColor(const QString &newColor) override;
    const QByteArray &seal() const override;
    void setSeal(const QByteArray &newSeal) override;
    const QString &phone() const override;
    void setPhone(const QString &newPhone) override;
    const QString &telegramm() const override;
    void setTelegramm(const QString &newTelegramm) override;
    const QString &instagramm() const override;
    void setInstagramm(const QString &newInstagramm) override;
    const QString &physicalAddress() const override;
    void setPhysicalAddress(const QString &newPhysicalAddress) override;
    const QString &webSite() const override;
    void setWebSite(const QString &newWebSite) override;
    const QString &title() const override;
    void setTitle(const QString &newTitle) override;
    const QByteArray &background() const override;
    void setBackground(const QByteArray &newBackground) override;
    const QString &freeItemName() const override;
    void setFreeItemName(const QString &newFreeItemName) override;
    unsigned int getCardVersion() const override;
    void setCardVersion(unsigned int newCardVersion) override;
    void idGen();
    const QByteArray &ownerSignature() const override;
    void setOwnerSignature(const QByteArray &newOwnerSignature) override;
    bool isOvner(const QByteArray &userId) const override;

    QString toString() const override;
    bool fromSqlRecord(const QSqlRecord &q) override;
    QString table() const override;
    bool isValid() const override;

    QH::PKG::DBVariantMap variantMap() const override;
    bool compare(const QSharedPointer<iCard> &other) override;

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

    QString primaryKey() const override;
    QString primaryValue() const override;

private:

    QByteArray _id;
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
