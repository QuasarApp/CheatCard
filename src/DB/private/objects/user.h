//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef DB_USER_H
#define DB_USER_H

#include <dbobject.h>

#include <rci/objects/iuser.h>

namespace RC {
namespace DB {

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
class User: public RC::Interfaces::iUser, public QH::PKG::DBObject
{
    ONLY_DATABASE_PACKAGE

public:
    User();
    User(const QSharedPointer<Interfaces::iUser>& user);

    QH::PKG::DBObject *createDBObject() const override;

    QH::PKG::DBVariantMap variantMap() const override;
    bool isValid() const override;
    bool isAllKeysIsValid() const override;

    const QString &name() const override;
    void setName(const QString &newName) override;
    bool fromSqlRecord(const QSqlRecord &q) override;

    bool fSaller() const override;
    void setFSaller(bool newFSaller) override;

    const QByteArray &getKey() const override;
    const QString getSignature() const override;
    void setKey(const QByteArray &newKey) override;

    const QByteArray &secret() const override;
    void setSecret(const QByteArray &newSecret) override;

    void regenerateKeys(const QByteArray& newSecret = {}) override;

    QString toString() const override;

protected:
    QString primaryKey() const override;
    QString primaryValue() const override;
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;
    QString table() const override;

private:
    QByteArray randomArray() const;

    QByteArray _key;
    QByteArray _secret;

    QString _name;
    bool _fSaller = false;

};

}
}
#endif // DB_USER_H
