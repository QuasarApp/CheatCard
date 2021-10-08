//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "carddatarequest.h"
namespace RC {

bool CardDataRequest::isValid() const {
    return AbstractData::isValid() && cardId && _requestToken;
}

QDataStream &CardDataRequest::fromStream(QDataStream &stream) {
    AbstractData::fromStream(stream);

    stream >> cardId;
    stream >> _requestToken;

    return stream;
}

QDataStream &CardDataRequest::toStream(QDataStream &stream) const {
    AbstractData::toStream(stream);

    stream << cardId;
    stream << _requestToken;

    return stream;
}

unsigned int CardDataRequest::getCardId() const {
    return cardId;
}

void CardDataRequest::setCardId(unsigned int newCardId) {
    cardId = newCardId;
}

RC::CardDataRequest::CardDataRequest() {}

}
