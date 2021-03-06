//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include <dbobject.h>
#include "CheatCard/core_global.h"
#include "CheatCard/requesttoken.h"
#include <QSharedPointer>

#ifndef CARD_H
#define CARD_H

namespace RC {
namespace API {

/**
 * @brief The Card class
 */
class CHEATCARD_CORE_EXPORT Card: public QH::PKG::DBObject
{
    QH_PACKAGE(Card, "Card")
public:
    Card();
    QH::PKG::DBObject *createDBObject() const override;

    QH::PKG::DBVariantMap variantMap() const override;
    bool isValid() const override;
    bool fromSqlRecord(const QSqlRecord &q) override;

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
    void setTelegramm(const QString &newTelegramm);

    const QString &instagramm() const;
    void setInstagramm(const QString &newInstagramm);

    const QString &physicalAddress() const;
    void setPhysicalAddress(const QString &newPhysicalAddress);

    const QString &webSite() const;
    void setWebSite(const QString &newWebSite);

    const QString &title() const;
    void setTitle(const QString &newTitle);

    const QByteArray &background() const;
    void setBackground(const QByteArray &newBackground);

    unsigned int cardId() const;

    const QString &freeItemName() const;
    void setFreeItemName(const QString &newFreeItemName);

    unsigned int getCardVersion() const;
    void setCardVersion(unsigned int newCardVersion);

    void idGen();

    const QByteArray &ownerSignature() const;
    void setOwnerSignature(const QByteArray &newOwnerSignature);

    bool isOvner(const unsigned int userId) const;

protected:
    QString primaryKey() const override;
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

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
Q_DECLARE_METATYPE(RC::API::Card)
Q_DECLARE_METATYPE(QSharedPointer<RC::API::Card>)

#endif // CARD_H
