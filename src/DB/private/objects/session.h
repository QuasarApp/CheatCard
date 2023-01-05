//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef DB_SESSION_H
#define DB_SESSION_H

#include "dbobject.h"

#include <rci/objects/isession.h>

namespace RC {
namespace DB {

/**
 * @brief The Session class is Database implementation of the iSession interface
 */
class Session: public RC::Interfaces::iSession, public QH::PKG::DBObject
{
    ONLY_DATABASE_PACKAGE

public:
    Session();
    Session(const QSharedPointer<Interfaces::iSession> &session);

    // AbstractData interface
public:
    bool isValid() const override;
    QH::PKG::DBObject *createDBObject() const override;

    long long getSessionId() const override;
    void setSessionId(long long newSessionId) override;

    bool fromSqlRecord(const QSqlRecord &q) override;
    QH::PKG::DBVariantMap variantMap() const override;
    long long getUsercardId() const override;
    void setUsercardId(long long newUsercardId) override;

protected:
    QString primaryKey() const override;
    QString primaryValue() const override;

    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;
    QString condition() const override;
    QString table() const override;

private:
    long long usercardId = 0;
    long long sessionId = 0;

};

}
}

#endif // DB_SESSION_H
