//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef USER_H
#define USER_H

#include "heart.h"
#include "CheatCard/core_global.h"

#include <dbobject.h>

namespace RC {
namespace API {

/**
 * @brief The User class This is maic class for contatins all information about user
 * The user structure is:
 * Secreet - value with that user will be prove to the server that it is owner.
 * User Key - this is hash of secret value (unique user key)
 * user id - short user id using for database key.
 *
 * ## Generate seteps:
 *
 * make secret
 * from secrete make key
 * from key make id.
 */
class CHEATCARD_CORE_EXPORT User: public QH::PKG::DBObject
{
    QH_PACKAGE(User, "CheatCardUser")

public:
    User();

    QH::PKG::DBObject *createDBObject() const override;

    QH::PKG::DBVariantMap variantMap() const override;
    bool isValid() const override;
    const QString &name() const;
    void setName(const QString &newName);
    bool fromSqlRecord(const QSqlRecord &q) override;

    bool fSaller() const;
    void setFSaller(bool newFSaller);

    const QByteArray &getKey() const;
    const QString getSignature() const;
    void setKey(const QByteArray &newKey);

    unsigned int userId() const;
    const QByteArray &secret() const;
    void setSecret(const QByteArray &newSecret);

    static QByteArray makeKey(const QByteArray& secret);
    static unsigned int makeId(const QByteArray& key);
    void regenerateKeys();

protected:
    QString primaryKey() const override;
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:

    QByteArray randomArray() const;

    QByteArray _key;
    QByteArray _secret;

    QString _name;
    bool _fSaller = false;
};

}
}
#endif // USER_H
