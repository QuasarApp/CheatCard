//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef USER_H
#define USER_H

#include "heart.h"
#include "CheatCard/core_global.h"

#include <dbobject.h>

namespace RC {

/**
 * @brief The User class This is maic class for contatins all information about user
 */
class CHEATCARD_CORE_EXPORT User: public QH::PKG::DBObject
{
    QH_PACKAGE(User, "CheatCardUser")

public:
    User();

    QH::PKG::DBObject *createDBObject() const override;

    QH::PKG::DBVariantMap variantMap() const override;

    const QString &name() const;
    void setName(const QString &newName);
    bool fromSqlRecord(const QSqlRecord &q) override;

    bool fSaller() const;
    void setFSaller(bool newFSaller);

    const QByteArray &getKey() const;
    void setKey(const QByteArray &newKey);

    unsigned int userId() const;
protected:
    QString primaryKey() const override;

private:

    QByteArray randomArray() const;

    QByteArray _key;
    QString _name;
    bool _fSaller = false;
};

}
#endif // USER_H
