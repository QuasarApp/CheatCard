//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef VISITORSSL_H
#define VISITORSSL_H

#include "visitor.h"

namespace RC {


class CHEATCARD_CORE_EXPORT VisitorSSL: public Visitor
{
    Q_OBJECT
public:
    VisitorSSL(QH::ISqlDBCache *db);

};

}
#endif // VISITORSSL_H
