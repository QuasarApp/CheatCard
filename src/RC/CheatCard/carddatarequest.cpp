//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "carddatarequest.h"
namespace RC {

bool CardDataRequest::isValid() const {
    return AbstractData::isValid() && cardIds.size() && requestToken();
}

QDataStream &CardDataRequest::fromStream(QDataStream &stream) {
    AbstractData::fromStream(stream);
    RequestToken::fromStream(stream);

    stream >> cardIds;

    return stream;
}

QDataStream &CardDataRequest::toStream(QDataStream &stream) const {
    AbstractData::toStream(stream);
    RequestToken::toStream(stream);

    stream << cardIds;

    return stream;
}

const QList<unsigned int> & CardDataRequest::getCardIds() const {
    return cardIds;
}

void CardDataRequest::setCardIds(const QList<unsigned int> &newCardId) {
    cardIds = newCardId;
}

void CardDataRequest::push(unsigned int id) {
    cardIds.push_back(id);
}

RC::CardDataRequest::CardDataRequest() {}

}
