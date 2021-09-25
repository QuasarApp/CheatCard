//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef SESSION_H
#define SESSION_H

#include "dbobject.h"

namespace RC {

class Session : public QH::PKG::DBObject
{
public:
    Session();

    // AbstractData interface
public:
    bool isValid() const override;
    QH::PKG::DBObject *createDBObject() const override;

    unsigned long long getSessionId() const;
    void setSessionId(unsigned long long newSessionId);

    bool fromSqlRecord(const QSqlRecord &q) override;
    QH::PKG::DBVariantMap variantMap() const override;
    unsigned long long getUsercardId() const;
    void setUsercardId(unsigned long long newUsercardId);

protected:
    QString primaryKey() const override;
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    unsigned long long usercardId = 0;
    unsigned long long sessionId;

};

}
#endif // SESSION_H
