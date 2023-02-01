//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef DBv0_USER_H
#define DBv0_USER_H

#include <dbobject.h>

#include <rci/objects/iuser.h>

namespace RC {
namespace DBv0 {

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
class User: public QH::PKG::DBObject
{
    ONLY_DATABASE_PACKAGE

public:
    User();
    User(const QSharedPointer<Interfaces::iUser>& user);

    QH::PKG::DBObject *createDBObject() const override;

    QH::PKG::DBVariantMap variantMap() const override;
    bool isValid() const override;
    bool isAllKeysIsValid() const;

    const QString &name() const;
    void setName(const QString &newName);
    bool fromSqlRecord(const QSqlRecord &q) override;

    bool fSaller() const;
    void setFSaller(bool newFSaller);

    const QByteArray &getKey() const;
    const QString getSignature() const;
    void setKey(const QByteArray &newKey);

    const QByteArray &secret() const;
    void setSecret(const QByteArray &newSecret);

    void regenerateKeys(const QByteArray& newSecret = {});

    unsigned int id() const;
    void setId(unsigned int newId);
    QString toString() const override;

protected:
    QString primaryKey() const override;
    QString primaryValue() const override;
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;
    QString table() const override;

private:
    QByteArray randomArray() const;

    unsigned int _id = 0;

    QByteArray _key;
    QByteArray _secret;

    QString _name;
    bool _fSaller = false;

};

}
}
#endif // DB_USER_H
