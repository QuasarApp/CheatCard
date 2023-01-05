//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef CARDUPDATED_H
#define CARDUPDATED_H
#include "abstractdata.h"

namespace RC {
namespace API {
namespace V2 {


/**
 * @brief The CardUpdated class is command for send notification about update card object to server.
 */
class CardUpdated: public QH::PKG::AbstractData
{
    QH_PACKAGE(CardUpdated, "APIv2::CardUpdated")

public:
    CardUpdated();

    // StreamBase interface
    unsigned int cardVersion() const;
    void setCardVersion(unsigned int newCardVersion);

    unsigned int cardId() const;
    void setCardId(unsigned int newCardId);
    bool isValid() const override;

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    unsigned int _cardVersion = 0;
    unsigned int _cardId = 0;

};

}
}
}
#endif // CARDUPDATED_H
