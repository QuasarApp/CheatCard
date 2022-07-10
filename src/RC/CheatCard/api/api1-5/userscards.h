//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef USERSCARDS_V1_5_H
#define USERSCARDS_V1_5_H

#include <CheatCard/api/api1/userscards.h>

namespace RC {
namespace APIv1_5 {


class CHEATCARD_CORE_EXPORT UsersCards: public APIv1::UsersCards
{
    QH_PACKAGE_AUTO(APIv1_5::UsersCards)

public:
    UsersCards();
    UsersCards(unsigned int user, unsigned int card);
    QH::PKG::DBObject *createDBObject() const override;


protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;
};

}
}
Q_DECLARE_METATYPE(RC::APIv1_5::UsersCards)
Q_DECLARE_METATYPE(QSharedPointer<RC::APIv1_5::UsersCards>)

#endif // USERSCARDS_V1_H
