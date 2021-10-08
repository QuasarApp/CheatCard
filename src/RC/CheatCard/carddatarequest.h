//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CARDDATAREQUEST_H
#define CARDDATAREQUEST_H

#include "requesttoken.h"

#include <abstractdata.h>

namespace RC {


class CHEATCARD_CORE_EXPORT CardDataRequest: public QH::PKG::AbstractData, public RequestToken{
    QH_PACKAGE(CardDataRequest, "CardDataRequest")

    // AbstractData interface
public:
    CardDataRequest();
    bool isValid() const override;
    // StreamBase interface

    unsigned int getCardId() const;
    void setCardId(unsigned int newCardId);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    unsigned int cardId = 0;
};
}
#endif // CARDDATAREQUEST_H
