//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef USER_H
#define USER_H

#include "heart.h"

#include <dbobject.h>

namespace RC {

/**
 * @brief The User class This is maic class for contatins all information about user
 */
class User: public QH::PKG::DBObject
{
public:
    User();

    // DBObject interface
public:
    QH::PKG::DBObject *createDBObject() const;

    QH::PKG::DBVariantMap variantMap() const;

    /**
     * @brief name This is name of user;
     * @return return na,e of user.
     */
    const QString &name() const;
    void setName(const QString &newName);

    /**
     * @brief cardID This is id of the user card.
     * @return user card id .
     */
    int cardID() const;
    void setCardID(int newCardID);

protected:
    QString primaryKey() const;

private:
    QString _name;
    int _cardID;
};

}
#endif // USER_H
