//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef NODEINFO_H
#define NODEINFO_H

#include <abstractnodeinfo.h>
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

private:
    unsigned long long _token;
};
}
#endif // NODEINFO_H
