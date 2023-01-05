//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "api0/card.h"

#ifndef CARD_V1_H
#define CARD_V1_H

namespace RC {
namespace API {
namespace V1 {

/**
 * @brief The Card class
 */
class Card: public API::V0::Card
{
    QH_PACKAGE(API::V1::Card, "APIv1::Card")
public:
    Card();
    Card(const QSharedPointer<Interfaces::iCard>& obj);

    QString toString() const override;

    QSharedPointer<RC::Interfaces::iCard>
    toObject(const QSharedPointer<Interfaces::iDB>& db) override;

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

};

}
}
}

#endif // CARD_V1_H
