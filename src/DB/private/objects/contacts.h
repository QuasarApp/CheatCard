//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef DB_SALLERSUSERS_H
#define DB_SALLERSUSERS_H

#include "rci/core/idb.h"
#include <dbobject.h>
#include <rci/objects/icontacts.h>

namespace RC {
namespace DB {

/**
 * @brief The Contacts class is Database implementation of the iContacts interface
 */
class Contacts: public RC::Interfaces::iContacts, public QH::PKG::DBObject
{
    ONLY_DATABASE_PACKAGE

public:
    Contacts();
    Contacts(const QSharedPointer<Interfaces::iContacts> &obj);

    // DBObject interface
    QH::PKG::DBObject *createDBObject() const override;
    QH::PKG::DBVariantMap variantMap() const override;
    bool isValid() const override;

    bool fromSqlRecord(const QSqlRecord &q) override;
    const QString &getInfo() const override;
    void setInfo(const QString &newInfo) override;

    const QByteArray &getChildUserKey() const override;
    void setChildUserKey(const QByteArray &newChildUserKey) override;

    const QByteArray &getUserKey() const override;
    void setUserKey(const QByteArray &newUserKey) override;
    QString table() const override;

    virtual QSharedPointer<RC::Interfaces::iContacts>
    toObject(const QSharedPointer<Interfaces::iDB> &db);

protected:

    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;
    std::pair<QString, QMap<QString, QVariant>> condition() const override;

private:
    QByteArray childUserKey;
    QByteArray userKey;
    QString info;

};

}
}

#endif // DB_SALLERSUSERS_H
