//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SELLER_H
#define SELLER_H

#include <dbobject.h>
#include <qqml.h>

namespace RC {

/**
 * @brief The Seller class
 */
class Saller : public QObject, public QH::PKG::DBObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int cardID READ getCardID WRITE setCardID NOTIFY cardIDChanged)

    QML_ELEMENT
public:
    Saller();

    QH::PKG::DBObject *createDBObject() const;
    QH::PKG::DBVariantMap variantMap() const;

    const QString &getName() const;
    void setName(const QString &newName);

    int getCardID() const;
    void setCardID(int newCardID);

signals:
    void nameChanged();

    void cardIDChanged();

protected:
    QString primaryKey() const;

private:
    QString name;
    int cardID;
};

}

#endif // SELLER_H
