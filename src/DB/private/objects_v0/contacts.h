//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef DBv0_SALLERSUSERS_H
#define DBv0_SALLERSUSERS_H

#include "rci/core/idb.h"
#include <dbobject.h>
#include <rci/objects/icontacts.h>

namespace RC {
namespace DBv0 {

/**
 * @brief The Contacts class is Database implementation of the iContacts interface
 */
class Contacts: public QH::PKG::DBObject
{
    ONLY_DATABASE_PACKAGE

public:
    Contacts();

    // DBObject interface
    QH::PKG::DBObject *createDBObject() const override;
    QH::PKG::DBVariantMap variantMap() const override;
    bool isValid() const override;

    bool fromSqlRecord(const QSqlRecord &q) override;
    const QString &getInfo() const;
    void setInfo(const QString &newInfo) ;

    const QByteArray &getChildUserKey() const ;
    void setChildUserKey(const QByteArray &newChildUserKey) ;

    const QByteArray &getUserKey() const ;
    void setUserKey(const QByteArray &newUserKey) ;
    QString table() const override;

    virtual QSharedPointer<RC::Interfaces::iContacts>
    toObject(const QSharedPointer<Interfaces::iDB> &db);

protected:
    QString primaryKey() const override;
    QVariant primaryValue() const override;

    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;
    std::pair<QString, QMap<QString, QVariant> > condition() const override;

private:
    QByteArray childUserKey;
    QByteArray userKey;
    QString info;

};

}
}

#endif // DB_SALLERSUSERS_H
