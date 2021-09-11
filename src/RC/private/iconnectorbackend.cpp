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

namespace RC {


IConnectorBackEnd::IConnectorBackEnd() {

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
    QH::Package pkg;

    pkg.fromBytes(message);

    if (!pkg.isValid()) {
        QuasarAppUtils::Params::log("Received invalid package", QuasarAppUtils::Error);

        return;
    }

    if (pkg.hdr.command == UserId) {

        if (message.size() != sizeof(UserHeader)) {
            QuasarAppUtils::Params::log("user id is invalid", QuasarAppUtils::Error);
            return;
        }

        // very dangerous
        UserHeader user = *reinterpret_cast<UserHeader*>(const_cast<char*>((message.data())));

        if (user.userId == 0) {
            QuasarAppUtils::Params::log("user id is invalid", QuasarAppUtils::Error);
            return;
        }

        if (_mode == Saller) {
            CardStatus status;
            status.cardId = _activeCard->cardId();
//            status.purchasesCount =
//            ;
        }
    }
}

QSharedPointer<Card> IConnectorBackEnd::activeCard() const {
    return _activeCard;
}

void IConnectorBackEnd::setActiveCard(QSharedPointer<Card> newActiveCard) {
    _activeCard = newActiveCard;
}

}

