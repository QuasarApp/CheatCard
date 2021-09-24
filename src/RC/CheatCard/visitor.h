//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef VISITOR_H
#define VISITOR_H

#include "iconnectorbackend.h"

namespace RC {


class Visitor: public IConnectorBackEnd
{
    Q_OBJECT
public:
    Visitor(DB *db, QSharedPointer<User> newActiveUser);

    // IConnectorBackEnd interface
protected:
    bool hello() override;
};
}
#endif // VISITOR_H
