//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef USERSCARDS_1_H
#define USERSCARDS_1_H

#include <dbobject.h>
#include "CheatCard/core_global.h"
#include <QDateTime>
#include <QSharedPointer>
#include <CheatCard/api/api0/userscards.h>

namespace RC {


class CHEATCARD_CORE_EXPORT UsersCardsV1: public UsersCards
{
    QH_PACKAGE(UsersCardsV1, "UsersCardsV1")

public:
    UsersCardsV1();
    UsersCardsV1(unsigned int user, unsigned int card);

    QH::PKG::DBObject *createDBObject() const override;
protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;
};

}

Q_DECLARE_METATYPE(RC::UsersCardsV1)
Q_DECLARE_METATYPE(QSharedPointer<RC::UsersCardsV1>)

#endif // USERSCARDS_1_H
