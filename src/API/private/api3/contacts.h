//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef SALLERSUSERS_V3_H
#define SALLERSUSERS_V3_H
#include "idataconvertor.h"
#include "rci/core/idb.h"
#include <dbobject.h>
#include <QSharedPointer>

#include <rci/objects/icontacts.h>

namespace RC {
namespace API {
namespace V3 {

class User;

class Contacts: public QH::PKG::AbstractData, public iDataConvertor<RC::Interfaces::iContacts>
{
    QH_PACKAGE_AUTO(RC::API::V3::Contacts)

public:
    Contacts();
    Contacts(const QSharedPointer<Interfaces::iContacts>&);

    bool isValid() const override;

    const QString &getInfo() const;
    void setInfo(const QString &newInfo);

    const QByteArray &getChildUserKey() const;
    void setChildUserKey(const QByteArray &newChildUserKey);

    const QByteArray &getUserKey() const;
    void setUserKey(const QByteArray &newUserKey);

    virtual QSharedPointer<RC::Interfaces::iContacts>
    toObject(const QSharedPointer<Interfaces::iDB> &db) const override;
    void fromObject(const QSharedPointer<RC::Interfaces::iContacts>& obj) override;

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QByteArray childUserKey;
    QByteArray userKey;
    QString info;

};

}
}
}

#endif // SALLERSUSERS_H
