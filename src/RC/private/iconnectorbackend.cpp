//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "iconnectorbackend.h"
#include "quasarapp.h"
#include "user.h"
#include "card.h"
#include "datastructures.h"
#include "itargetnode.h"
#include "userscards.h"
#include "dbobjectsrequest.h"
#include "getsinglevalue.h"

namespace RC {


IConnectorBackEnd::IConnectorBackEnd(DB *db) {
    _db = db;
}

bool IConnectorBackEnd::start(Mode mode) {
    return listen(mode);
}

bool IConnectorBackEnd::stop() {
    return close();
}

void IConnectorBackEnd::connectionReceived(ITargetNode *obj) {

    _currentTarget = QSharedPointer<ITargetNode>(obj);
}

void IConnectorBackEnd::connectionLost( ITargetNode *id) {
    if (_currentTarget != id) {
        QuasarAppUtils::Params::log("Try drop another connection!!", QuasarAppUtils::Error);
    }

    _currentTarget.reset();
}

void IConnectorBackEnd::handleReceiveMessage(const QByteArray &message) {

    unsigned char command = *message.begin();

    switch (command) {
    case UserId : {
        if (!workWithUserRequest(message)) {
            QuasarAppUtils::Params::log("Failed to parse package", QuasarAppUtils::Error);

        }
    }

    }
}

bool IConnectorBackEnd::workWithUserRequest(const QByteArray &message) {
    if (message.size() != sizeof(UserHeader)) {
        QuasarAppUtils::Params::log("user id is invalid", QuasarAppUtils::Error);
        return false;
    }

    // very dangerous
    UserHeader user = *reinterpret_cast<UserHeader*>(const_cast<char*>((message.data())));

    if (user.userId == 0) {
        QuasarAppUtils::Params::log("user id is invalid", QuasarAppUtils::Error);
        return false;
    }

    if (_mode == Saller) {
        User userrquest;
        userrquest.setId(user.userId);

        auto dbUser = _db->getObject(userrquest);

        if (!dbUser) {

            DataRequest responce;
            responce.command = UserDataRequest;

            _currentTarget->sendMessage(QByteArray::fromRawData(reinterpret_cast<char*>(&responce), sizeof(responce)));
            return true;
        }

        CardStatus status;
        status.cardId = _activeCard->cardId();

        QString where = QString("user = %0 AND card = %1)").
                arg(user.userId).
                arg(status.cardId);
        QH::PKG::DBObjectsRequest<UsersCards> requestPurchase("UsersCards", where);

        auto purches = _db->getObject(requestPurchase);
        if (!purches->data().size()) {
            status.purchasesCount = purches->data().first()->getPurchasesNumber();
        }

        status.purchasesCount++;

        _currentTarget->sendMessage(QByteArray::fromRawData(reinterpret_cast<char*>(&status), sizeof(status)));
        return true;
    }

    return false;
}

QSharedPointer<Card> IConnectorBackEnd::activeCard() const {
    return _activeCard;
}

void IConnectorBackEnd::setActiveCard(QSharedPointer<Card> newActiveCard) {
    _activeCard = newActiveCard;
}

}

