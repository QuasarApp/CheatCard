//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef VISITOR_H
#define VISITOR_H

#include "basenode.h"

namespace RC {


class CHEATCARD_EXPORT Visitor: public BaseNode
{
    Q_OBJECT
public:
    Visitor(QH::ISqlDBCache *db);
    bool checkCardData(long long session,
                       const QString& domain = DEFAULT_CHEAT_CARD_HOST,
                       int port = DEFAULT_CHEAT_CARD_PORT);

    // AbstractNode interface
protected:
    void nodeConnected(QH::AbstractNodeInfo *node) override;

private:
    long long _lastRequested = 0;


};
}
#endif // VISITOR_H
