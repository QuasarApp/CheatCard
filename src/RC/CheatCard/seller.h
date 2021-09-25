//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef SELLER_H
#define SELLER_H
#include "basenode.h"

namespace RC {

class CheatCard_EXPORT Seller: public BaseNode
{
    Q_OBJECT
public:
    Seller(QH::ISqlDBCache *db);

    bool sendLastSession(unsigned long long session,
                         const QString& domain = DEFAULT_CHEAT_CARD_HOST,
                         int port = DEFAULT_CHEAT_CARD_PORT);
protected:
    void nodeConfirmend(QH::AbstractNodeInfo *node) override;

private:
    unsigned long long _lastRequested = 0;
};
}
#endif // SELLER_H
