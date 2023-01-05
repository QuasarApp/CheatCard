//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef USERSCARDS_V1_H
#define USERSCARDS_V1_H

#include "api0/userscards.h"

namespace RC {
namespace API {
namespace V1 {

class UsersCards: public API::V0::UsersCards
{
    QH_PACKAGE(API::V1::UsersCards, "APIv1::UsersCards")

public:
    UsersCards();
    UsersCards(unsigned int user, unsigned int card);
    UsersCards(const QSharedPointer<Interfaces::iUsersCards>& obj);

    QString toString() const override;

    QSharedPointer<RC::Interfaces::iUsersCards>
    toObject(const QSharedPointer<Interfaces::iDB> &db) override;
protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;
};

}
}
}

#endif // USERSCARDS_V1_H
