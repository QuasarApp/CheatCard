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

    QH::PKG::DBObject *createDBObject() const;

    QH::PKG::DBVariantMap variantMap() const;

    const QString &name() const;
    void setName(const QString &newName);
    bool fromSqlRecord(const QSqlRecord &q);

    bool fSaller() const;
    void setFSaller(bool newFSaller);

    const QString &visibleName() const;
    void setVisibleName(const QString &newVisibleName);

    const QByteArray &getKey() const;
    void setKey(const QByteArray &newKey);

protected:
    QString primaryKey() const;

private:

    QByteArray randomArray() const;

    QByteArray _key;
    QString _name;
    QString _visibleName;
    bool _fSaller;
};

}
#endif // USER_H
