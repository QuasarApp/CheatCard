//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include <CheatCard/api/api0/card.h>

#ifndef CARD_V1_H
#define CARD_V1_H

namespace RC {
namespace APIv1 {

/**
 * @brief The Card class
 */
class CHEATCARD_CORE_EXPORT Card: public API::Card
{
    QH_PACKAGE_AUTO(APIv1::Card)
public:
    Card();
    QH::PKG::DBObject *createDBObject() const override;

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

};

}
}

Q_DECLARE_METATYPE(RC::APIv1::Card)
Q_DECLARE_METATYPE(QSharedPointer<RC::APIv1::Card>)

#endif // CARD_V1_H
