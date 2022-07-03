//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SALLERSUSERS_H
#define SALLERSUSERS_H
#include <dbobject.h>
#include "CheatCard/core_global.h"


namespace RC {
namespace API {
class User;

class CHEATCARD_CORE_EXPORT Contacts: public QH::PKG::DBObject
{
    QH_PACKAGE_AUTO(RC::API::Contacts)

public:
    Contacts();
    // DBObject interface
    QH::PKG::DBObject *createDBObject() const override;
    QH::PKG::DBVariantMap variantMap() const override;
    bool isValid() const override;

    bool fromSqlRecord(const QSqlRecord &q) override;
    const QString &getInfo() const;
    void setInfo(const QString &newInfo);

    unsigned int getUser() const;

    unsigned int getChildUserId() const;

    int getGenesisKey() const;
    void setGenesisKey(int newGenesisKey);

    const QByteArray &getChildUserKey() const;
    void setChildUserKey(const QByteArray &newChildUserKey);

    const QByteArray &getUserKey() const;
    void setUserKey(const QByteArray &newUserKey);

    QSharedPointer<API::User> toUser(const QSharedPointer<API::User>& currentUser) const;
protected:
    QString primaryKey() const override;
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;
    QString condition() const override;

private:
    QByteArray childUserKey;

    QByteArray userKey;
    int genesisKey = 0;

    QString info;

};

}
}
#endif // SALLERSUSERS_H
