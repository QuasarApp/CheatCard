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

    const QString &name() const;
    void setName(const QString &newName);

    const QByteArray &logo() const;
    void setLogo(const QByteArray &newImage);

    int getPurchasesNumber() const;
    void setPurchasesNumber(int newPurchasesNumber);

    int getFreeIndex() const;
    void setFreeIndex(int newFreeIndex);

    const QColor &getColor() const;
    void setColor(const QColor &newColor);

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

protected:
    QString primaryKey() const override;

private:
    QString _name;
    QString _title;
    QByteArray _logo;
    QByteArray _seal;
    QString _phone;
    QString _telegramm;
    QString _instagramm;
    QString _physicalAddress;
    QString _webSite;

    int purchasesNumber = 1;
    int freeIndex = 1;
    QColor color = QColor("#777777");

};

}
#endif // CARD_H
