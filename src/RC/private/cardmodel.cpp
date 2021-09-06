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

CardModel::CardModel(Card *card) {
    setCard(card);
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

void CardModel::setName(const QString &newName) {
    if (!_card)
        return;

    if (_card->name() == newName)
        return;

    _card->setName(newName);
}

void CardModel::setPurchasesNumber(int newPurchasesNumber) {
    if (!_card)
        return;

    if (_card->getPurchasesNumber() == newPurchasesNumber)
        return;

    _card->setPurchasesNumber(newPurchasesNumber);
}

void CardModel::setFreeIndex(int newFreeIndex) {

    if (!_card)
        return;

    if (_card->getFreeIndex() == newFreeIndex)
        return;

    _card->setFreeIndex(newFreeIndex);
}

void CardModel::save() {
    if (_card)
        emit editFinished(_card);
}

QString CardModel::phone() const {
    if (!_card)
        return "";
    return _card->phone();
}

void CardModel::setPhone(const QString &newPhone) {
    if (!_card)
        return;

    if (_card->phone() == newPhone)
        return;

    _card->setPhone(newPhone);
}

QString CardModel::telegramm() const {
    if (!_card)
        return "";
    return _card->telegramm();
}

void CardModel::setTelegramm(const QString &newTelegramm) {
    if (!_card)
        return;

    if (_card->telegramm() == newTelegramm)
        return;

    _card->setTelegramm(newTelegramm);
}

QString CardModel::instagramm() const {
    if (!_card)
        return "";
    return _card->instagramm();
}

void CardModel::setInstagramm(const QString &newInstagramm) {
    if (!_card)
        return;

    if (_card->telegramm() == newInstagramm)
        return;

    _card->setInstagramm(newInstagramm);
}

QString CardModel::physicalAddress() const {
    if (!_card)
        return "";
    return _card->physicalAddress();
}

void CardModel::setPhysicalAddress(const QString &newPhysicalAddress) {
    if (!_card)
        return;

    if (_card->physicalAddress() == newPhysicalAddress)
        return;

    _card->setPhysicalAddress(newPhysicalAddress);
}

QString CardModel::webSite() const {
    if (!_card)
        return "";
    return _card->webSite();
}

void CardModel::setWebSite(const QString &newWebSite) {
    if (!_card)
        return;

    if (_card->webSite() == newWebSite)
        return;

    _card->setWebSite(newWebSite);
}

QString CardModel::getColor() const {
    if (!_card)
        return "#777777";
    return _card->getColor().name();
}

void CardModel::setColor(const QString &newColor) {
    if (!_card)
        return;

    if (_card->getColor() == newColor)
        return;

    _card->setColor(newColor);
}

QString CardModel::title() const {
    if (!_card)
        return "";
    return _card->title();
}

void CardModel::setTitle(const QString &newTitle) {
    if (!_card)
        return;

    if (_card->title() == newTitle)
        return;

    _card->setTitle(newTitle);
}



}
