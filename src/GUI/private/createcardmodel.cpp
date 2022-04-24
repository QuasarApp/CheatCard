//#
//# Copyright (C) 2022-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "createcardmodel.h"
namespace RC {

CreateCardModel::CreateCardModel() {
    _currentCard = new CardModel();

    setCurrentCard(QSharedPointer<API::Card>::create());

    connect(_currentCard, &CardModel::editFinished,
            this, &CreateCardModel::handleCardEditFinished);
}

CreateCardModel::~CreateCardModel() {
    delete _currentCard;
}

QObject *CreateCardModel::currentCard() const {
    return _currentCard;
}

void CreateCardModel::handleCardEditFinished(const QSharedPointer<API::Card>&) {
    if (!(_currentCard->card() && _currentCard->card()->isValid()))
        return;

    emit sigCardCreated(_currentCard->card());
    setCurrentCard(QSharedPointer<API::Card>::create());
}

void CreateCardModel::setCurrentCard(const QSharedPointer<API::Card>& card) {
    _currentCard->setCard(card);
    emit currentCardChanged();
}

}
