//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "carddatarequest.h"
namespace RC {

bool CardDataRequest::isValid() const {
    return AbstractData::isValid() && cardIds.size() && _requestToken;
}

QDataStream &CardDataRequest::fromStream(QDataStream &stream) {
    AbstractData::fromStream(stream);

    stream >> cardIds;
    stream >> _requestToken;

    return stream;
}

QDataStream &CardDataRequest::toStream(QDataStream &stream) const {
    AbstractData::toStream(stream);

    stream << cardIds;
    stream << _requestToken;

    return stream;
}

const QList<unsigned int> & CardDataRequest::getCardId() const {
    return cardIds;
}

void CardDataRequest::setCardId(const QList<unsigned int> &newCardId) {
    cardIds = newCardId;
}

void CardDataRequest::push(unsigned int id) {
    cardIds.push_back(id);
}

RC::CardDataRequest::CardDataRequest() {}

}
