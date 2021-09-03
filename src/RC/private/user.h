//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef USER_H
#define USER_H

#include "heart.h"
#include <qqml.h>

#include <dbobject.h>

namespace RC {

/**
 * @brief The User class This is maic class for contatins all information about user
 */
class User: public QObject, public QH::PKG::DBObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int cardID READ cardID WRITE setCardID NOTIFY cardIDChanged)

    QML_ELEMENT
public:
    User();

    // DBObject interface
public:
    QH::PKG::DBObject *createDBObject() const;

    QH::PKG::DBVariantMap variantMap() const;

    const QString &name() const;
    void setName(const QString &newName);

    int cardID() const;
    void setCardID(int newCardID);

signals:
    void nameChanged();

    void cardIDChanged();

protected:
    QString primaryKey() const;

private:
    QString _name;
    int _cardID;
};

}
#endif // USER_H
