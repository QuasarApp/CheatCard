//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef CARDDATAREQUEST_H
#define CARDDATAREQUEST_H

#include <abstractdata.h>

namespace RC {
namespace API {
namespace V3 {


class CardDataRequest: public QH::PKG::AbstractData {
    QH_PACKAGE_AUTO(API::V3::CardDataRequest)

    // AbstractData interface
public:
    CardDataRequest();
    bool isValid() const override;
    // StreamBase interface

    const QList<QByteArray>& getCardIds() const;
    void setCardIds(const QList<QByteArray>& newCardIds);
    void push(const QByteArray& id);

protected:
    QDataStream &fromStream(QDataStream &stream) override;
    QDataStream &toStream(QDataStream &stream) const override;

private:
    QList<QByteArray> cardIds;
};
}
}
}
#endif // CARDDATAREQUEST_H
