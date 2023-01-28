//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "rci/core/idb.h"
#include "rci/objects/icard.h"
#include <abstractdata.h>

#ifndef CARD_V3_H
#define CARD_V3_H

namespace RC {
namespace API {
namespace V3 {

/**
 * @brief The Card class is network object of the card
 */
class Card: public QH::PKG::AbstractData {
    QH_PACKAGE_AUTO(API::V3::Card)
public:
    Card();
    Card(const QSharedPointer<Interfaces::iCard>& obj);

    QString toString() const override;

    bool isValid() const override;

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

    const QString &freeItemName() const;
    void setFreeItemName(const QString &newFreeItemName);

    unsigned int getCardVersion() const;
    void setCardVersion(unsigned int newCardVersion);

    const QByteArray &ownerSignature() const;
    void setOwnerSignature(const QByteArray &newOwnerSignature);

    QSharedPointer<RC::Interfaces::iCard>
    toObject(const QSharedPointer<Interfaces::iDB>& db);

    unsigned int shortId() const;
    void setShortId(unsigned int newShortId);

    QByteArray key() const;
    void setKey(const QByteArray &newKey);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    unsigned int _shortId;
    QByteArray _key;
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
}

#endif // CARD_V1_H
