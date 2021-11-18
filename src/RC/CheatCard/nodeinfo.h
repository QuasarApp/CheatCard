//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef NODEINFO_H
#define NODEINFO_H

#include <abstractnodeinfo.h>
#include "applicationversion.h"
#include "core_global.h"
namespace RC {


class CHEATCARD_CORE_EXPORT NodeInfo : public QH::AbstractNodeInfo
{
    Q_OBJECT
public:
    NodeInfo(QAbstractSocket *sct = nullptr,
             const QH::HostAddress* address = nullptr);
    unsigned long long token() const;
    void setToken(unsigned long long newToken);

    const ApplicationVersion& version() const;
    void setVersion(const ApplicationVersion& newVersion);


    // AbstractNodeInfo interface
protected:
    bool confirmData() const override;

private:
    unsigned long long _token;
    ApplicationVersion _version;
    bool _fVersionReceived = false;

};
}
#endif // NODEINFO_H
