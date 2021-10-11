//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef CARDSTATUSREQUEST_H
#define CARDSTATUSREQUEST_H

#include <abstractdata.h>
#include "requesttoken.h"

namespace RC {

class CHEATCARD_CORE_EXPORT CardStatusRequest: public QH::PKG::AbstractData
{
    QH_PACKAGE(CardStatusRequest, "CardStatusRequest")
public:
    CardStatusRequest();
    bool isValid() const override;
    // StreamBase interface

    long long getSessionId() const;
    void setSessionId(long long newSessionId);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    long long sessionId;
};
}
#endif // CARDSTATUSREQUEST_H
