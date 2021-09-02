//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

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
    QH::PKG::DBObject *createDBObject() const;

    QH::PKG::DBVariantMap variantMap() const;

    const QString &name() const;
    void setName(const QString &newName);

    const QByteArray &image() const;
    void setImage(const QByteArray &newImage);

    int getPurchasesNumber() const;
    void setPurchasesNumber(int newPurchasesNumber);

    int getFreeIndex() const;
    void setFreeIndex(int newFreeIndex);

protected:
    QString primaryKey() const;

private:
    QString _name;
    QByteArray _image;
    int purchasesNumber = 0;
    int freeIndex = 1;

};

}
#endif // CARD_H
