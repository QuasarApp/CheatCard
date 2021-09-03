//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cardmodel.h"
#include "card.h"
namespace RC {
CardModel::CardModel() {

}

int CardModel::id() const {
    if (!_card)
        return -1;
    return _card->getId().toInt();
}

QString CardModel::name() const {
    if (!_card)
        return "";
    return _card->name();
}

int CardModel::purchasesNumber() const {
    if (!_card)
        return 0;
    return _card->getPurchasesNumber();
}

int CardModel::freeIndex() const {
    if (!_card)
        return 0;
    return _card->getFreeIndex();
}

Card *CardModel::card() const {
    return _card;
}

void CardModel::setCard(Card *newCard) {
    if (_card != newCard)
        return;
    _card = newCard;

    emit objChanged();
}

}
