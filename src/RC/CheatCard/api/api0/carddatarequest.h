//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CARDDATAREQUEST_H
#define CARDDATAREQUEST_H

#include "CheatCard/requesttoken.h"

#include <abstractdata.h>

namespace RC {
namespace API {


class CHEATCARD_CORE_EXPORT CardDataRequest: public QH::PKG::AbstractData, public RequestToken {
    QH_PACKAGE(CardDataRequest, "CardDataRequest")

    // AbstractData interface
public:
    CardDataRequest();
    bool isValid() const override;
    // StreamBase interface

    const QList<unsigned int>& getCardIds() const;
    void setCardIds(const QList<unsigned int>& newCardIds);
    void push(unsigned int id);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QList<unsigned int> cardIds;
};
}
}
#endif // CARDDATAREQUEST_H
