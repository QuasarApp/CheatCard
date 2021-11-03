//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "cardmodel.h"
#include "CheatCard/card.h"

#include <QBuffer>
#include <QPixmap>

#define MAXIMUM_IMAGE_SIZE 300.0f

namespace RC {
CardModel::CardModel() {

}

CardModel::CardModel(QSharedPointer<Card> card) {
    setCard(card);
}

unsigned int CardModel::id() const {
    if (!_card)
        return -1;
    return _card->cardId();
}

int CardModel::purchasesNumber() const {
    return _purchasesNumber;
}

int CardModel::freeIndex() const {
    if (!_card)
        return 0;
    return _card->getFreeIndex();
}

QSharedPointer<Card> CardModel::card() const {
    return _card;
}

void CardModel::setCard(const QSharedPointer<Card> &newCard) {
    if (_card == newCard)
        return;

    _card = newCard;

    emit objChanged();
}

void CardModel::setPurchasesNumber(int newPurchasesNumber) {
    if (_purchasesNumber == newPurchasesNumber) {
        return;
    }

    _purchasesNumber = newPurchasesNumber;

    emit purchasesNumberChanged();
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

void CardModel::setNewBackGround(const QString& backgroundPath) {

    if (!_card)
        return;

    _card->setBackground(convert(backgroundPath));
}

void CardModel::setNewLogo(const QString &logoPath) {
    if (!_card)
        return;

    _card->setLogo(convert(logoPath));
}

void CardModel::setNewSeel(const QString &seelPath) {
    if (!_card)
        return;

    _card->setSeal(convert(seelPath));
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
    return _card->getColor();
}

void CardModel::setColor(const QString &newColor) {
    if (!_card)
        return;

    if (_card->getColor() == newColor)
        return;

    _card->setColor(newColor);
}

QString CardModel::getFontColor() const {
    if (!_card)
        return "#777777";
    return _card->getFontColor();
}

void CardModel::setFontColor(const QString &newColor) {
    if (!_card)
        return;

    if (_card->getFontColor() == newColor)
        return;

    _card->setFontColor(newColor);
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

QByteArray CardModel::convert(const QString& imagePath) {
    QUrl url(imagePath);

#ifdef Q_OS_ANDROID
    QPixmap tmpImage((url.scheme() == "qrc")? ":" + QUrl(imagePath).path(): imagePath);
#else
    QPixmap tmpImage((url.scheme() == "qrc")? ":" + QUrl(imagePath).path(): QUrl(imagePath).path());
#endif

    float scaleFactor = 0;

    if (tmpImage.height() > MAXIMUM_IMAGE_SIZE) {
        scaleFactor = tmpImage.height() / MAXIMUM_IMAGE_SIZE;
    } else if (tmpImage.width() > MAXIMUM_IMAGE_SIZE) {
        scaleFactor = std::max(scaleFactor,
                               tmpImage.width() / static_cast<float>(MAXIMUM_IMAGE_SIZE));
    }

    if (scaleFactor > 0) {
        tmpImage = tmpImage.scaled( tmpImage.width() / scaleFactor,
                                    tmpImage.height() / scaleFactor,
                                    Qt::IgnoreAspectRatio,
                                    Qt::TransformationMode::SmoothTransformation);
    }

    QByteArray result;
    QBuffer buf(&result);
    if (buf.open(QIODevice::WriteOnly)) {
        tmpImage.save(&buf, "PNG");
        buf.close();
    }

    return result;;
}

QString CardModel::getFreeItem() const {
    if (!_card)
        return "";
    return _card->freeItemName();
}

void CardModel::setFreeItem(const QString &newFreeItem) {
    if (!_card)
        return;

    if (_card->freeItemName() == newFreeItem)
        return;

    _card->setFreeItemName(newFreeItem);
}

int CardModel::cardVersion() const {
    if (!_card)
        return 0;
    return _card->getCardVersion();
}


}
