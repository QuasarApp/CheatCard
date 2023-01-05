//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "deletecardrequest.h"

namespace RC {
namespace API {
namespace V2 {
DeleteCardRequest::DeleteCardRequest() {

}

bool DeleteCardRequest::isValid() const {
    return _card && _secret.size();
}

unsigned int DeleteCardRequest::card() const {
    return _card;
}

void DeleteCardRequest::setCard(unsigned int newCard) {
    _card = newCard;
}

const QByteArray &DeleteCardRequest::secret() const {
    return _secret;
}

void DeleteCardRequest::setSecret(const QByteArray &newSecret) {
    _secret = newSecret;
}

QDataStream &DeleteCardRequest::fromStream(QDataStream &stream) {
    stream >> _card;
    stream >> _secret;

    return stream;
}

QDataStream &DeleteCardRequest::toStream(QDataStream &stream) const {
    stream << _card;
    stream << _secret;

    return stream;

}
}
}
}
