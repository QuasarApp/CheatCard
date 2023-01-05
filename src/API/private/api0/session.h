//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef SESSION_H
#define SESSION_H

#include "rci/core/idb.h"
#include "rci/objects/isession.h"
#include <QSharedPointer>
#include <abstractdata.h>

namespace RC {
namespace API {
namespace V0 {

class Session : public QH::PKG::AbstractData
{
    QH_PACKAGE(Session, "Session")

public:
    Session();
    Session(const QSharedPointer<Interfaces::iSession>&);

    bool isValid() const override;

    long long getSessionId() const;
    void setSessionId(long long newSessionId);

    long long getUsercardId() const;
    void setUsercardId(long long newUsercardId);

    virtual QSharedPointer<RC::Interfaces::iSession>
    toObject(const QSharedPointer<Interfaces::iDB> &db);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    long long usercardId = 0;
    long long sessionId = 0;

};
}
}
}

#endif // SESSION_H
