#include "cardupdated.h"


namespace RC {
namespace APIv1_5 {
CardUpdated::CardUpdated() {

}

unsigned int CardUpdated::cardVersion() const {
    return _cardVersion;
}

void CardUpdated::setCardVersion(unsigned int newCardVersion) {
    _cardVersion = newCardVersion;
}

unsigned int CardUpdated::cardId() const {
    return _cardId;
}

void CardUpdated::setCardId(unsigned int newCardId) {
    _cardId = newCardId;
}

bool CardUpdated::isValid() const {
    return _cardId && _cardVersion;
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