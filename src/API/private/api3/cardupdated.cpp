//#
//# Copyright (C) 2023-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "cardupdated.h"


namespace RC {
namespace API {
namespace V3 {

CardUpdated::CardUpdated() {

}

unsigned int CardUpdated::cardVersion() const {
    return _cardVersion;
}

void CardUpdated::setCardVersion(unsigned int newCardVersion) {
    _cardVersion = newCardVersion;
}

const QByteArray &CardUpdated::cardId() const {
    return _cardId;
}

void CardUpdated::setCardId(const QByteArray &newCardId) {
    _cardId = newCardId;
}

bool CardUpdated::isValid() const {
    return _cardId.size() == 32 && _cardVersion;
}

QDataStream &CardUpdated::fromStream(QDataStream &stream) {
    stream >> _cardId;
    stream >> _cardVersion;

    return stream;
}

QDataStream &CardUpdated::toStream(QDataStream &stream) const {
    stream << _cardId;
    stream << _cardVersion;

    return stream;
}

}
}
}
