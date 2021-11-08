//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef SESSION_H
#define SESSION_H

#include "appversion.h"
#include "dbobject.h"

namespace RC {

class Session : public QH::PKG::DBObject, public AppVersion
{
    QH_PACKAGE(Session, "Session")

public:
    Session();

    // AbstractData interface
public:
    bool isValid() const override;
    QH::PKG::DBObject *createDBObject() const override;

    long long getSessionId() const;
    void setSessionId(long long newSessionId);

    bool fromSqlRecord(const QSqlRecord &q) override;
    QH::PKG::DBVariantMap variantMap() const override;
    long long getUsercardId() const;
    void setUsercardId(long long newUsercardId);

protected:
    QString primaryKey() const override;
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;
    QString condition() const override;

private:
    long long usercardId = 0;
    long long sessionId;



};

}
#endif // SESSION_H
