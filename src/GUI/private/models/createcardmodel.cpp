//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "createcardmodel.h"
#include "qaglobalutils.h"
#include "usermodel.h"
#include "userslistmodel.h"

#include <rci/core/idb.h>
#include <rci/core/imodelsstorage.h>

namespace RC {

CreateCardModel::CreateCardModel() {
    _currentCard = new CardModel();
    connect(_currentCard, &CardModel::editFinished,
            this, &CreateCardModel::handleCardEditFinished);
}

CreateCardModel::~CreateCardModel() {
    delete _currentCard;
}

QObject *CreateCardModel::currentCard() const {
    return _currentCard;
}

void CreateCardModel::init(const QSharedPointer<Interfaces::iDB> &db,
                           const QSharedPointer<Interfaces::iModelsStorage> &global) {
    BaseModel::init(db, global);
    setCurrentCard(db->makeEmptyCard());


}

void CreateCardModel::handleCardEditFinished(const QSharedPointer<Interfaces::iCard>&) {
    auto card = _currentCard->card();
    if (!card)
        return;

    auto user = getCurrentuser();
    if (user) {
        card->setOwnerSignature(user->getKey());
    }

    if (!(card->isValid()))
        return;

    emit sigCardCreated(_currentCard->card());
    if (auto _db = db()) {
        setCurrentCard(_db->makeEmptyCard());
    }
}

void CreateCardModel::setCurrentCard(const QSharedPointer<Interfaces::iCard>& card) {
    _currentCard->setCard(card);
    emit currentCardChanged();
}

QSharedPointer<Interfaces::iUser> CreateCardModel::getCurrentuser() {
    if (!_usersModel)
        _usersModel = storage()->get(typeid(UsersListModel).hash_code()).staticCast<UsersListModel>();

    debug_assert(_usersModel, "UsersListModel should be initialize first");

    if (!_usersModel->currentUser())
        return {};

    return _usersModel->currentUser()->user();
}

}
