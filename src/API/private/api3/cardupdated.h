//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef CARDUPDATED_V3_H
#define CARDUPDATED_V3_H
#include "abstractdata.h"

namespace RC {
namespace API {
namespace V3 {


/**
 * @brief The CardUpdated class is command for send notification about update card object to server.
 */
class CardUpdated: public QH::PKG::AbstractData
{
    QH_PACKAGE(CardUpdated, "APIv3::CardUpdated")

public:
    CardUpdated();

    // StreamBase interface
    unsigned int cardVersion() const;
    void setCardVersion(unsigned int newCardVersion);

    const QByteArray& cardId() const;
    void setCardId(const QByteArray& newCardId);
    bool isValid() const override;

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    unsigned int _cardVersion = 0;
    QByteArray _cardId;

};

}
}
}
#endif // CARDUPDATED_H
