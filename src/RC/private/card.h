//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include <QColor>
#include <dbobject.h>

#ifndef CARD_H
#define CARD_H

namespace RC {

/**
 * @brief The Card class
 */
class Card: public QH::PKG::DBObject
{
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

protected:
    QString primaryKey() const override;
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QString _title;
    QByteArray _logo;
    QByteArray _seal;
    QByteArray _background;

    QString _phone;
    QString _telegramm;
    QString _instagramm;
    QString _physicalAddress;
    QString _webSite;
    QString _freeItemName;

    int freeIndex = 1;
    QString color = "#777777";

};

}
#endif // CARD_H
