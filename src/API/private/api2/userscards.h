//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef USERSCARDS_V1_5_H
#define USERSCARDS_V1_5_H

#include <api1/userscards.h>

namespace RC {
namespace API {
namespace V2 {

class UsersCards: public API::V1::UsersCards
{
    QH_PACKAGE(RC::API::V2::UsersCards, "APIv2::UsersCards")

public:
    UsersCards();
    UsersCards(unsigned int user, unsigned int card);
    UsersCards(const QSharedPointer<Interfaces::iUsersCards>& obj);


protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;
};

}
}
}

#endif // USERSCARDS_V1_H
