//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "carddatarequest.h"
namespace RC {
namespace API {
namespace V3 {

bool CardDataRequest::isValid() const {
    return AbstractData::isValid() && cardIds.size();
}

QDataStream &CardDataRequest::fromStream(QDataStream &stream) {
    stream >> cardIds;

    return stream;
}

QDataStream &CardDataRequest::toStream(QDataStream &stream) const {
    stream << cardIds;

    return stream;
}

const QList<QByteArray> & CardDataRequest::getCardIds() const {
    return cardIds;
}

void CardDataRequest::setCardIds(const QList<QByteArray> &newCardId) {
    cardIds = newCardId;
}

void CardDataRequest::push(const QByteArray& id) {
    cardIds.push_back(id);
}

CardDataRequest::CardDataRequest() {}

}
}
}
