//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SELLER_H
#define SELLER_H

#include <dbobject.h>

namespace RC {

/**
 * @brief The Seller class
 */
class Saller : public QH::PKG::DBObject
{
public:
    Saller();

    const QString &getName() const;
    void setName(const QString &newName);

    int getCardID() const;
    void setCardID(int newCardID);

    QH::PKG::DBObject *createDBObject() const;
    QH::PKG::DBVariantMap variantMap() const;
protected:
    QString primaryKey() const;

private:
    QString name;
    int cardID;
};

}
#endif // SELLER_H
