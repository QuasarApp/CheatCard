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
#include <cstring>
#include <qmlnotifyservice.h>
#include <session.h>


#define TIME_LIMIT_SEC 10

namespace RC {


IConnectorBackEnd::~IConnectorBackEnd() {
    if(_lastReceivedCardStatus)
        delete _lastReceivedCardStatus;
}

bool IConnectorBackEnd::start(Mode mode) {
    return listen(mode);
}

bool IConnectorBackEnd::stop() {
    return close();
}

void IConnectorBackEnd::reset() {

    close();

    _currentTarget.reset();
}






IConnectorBackEnd::Error IConnectorBackEnd::lastStatus() const {
    return _lastStatus;
}

IConnectorBackEnd::Mode IConnectorBackEnd::mode() const {
    return _mode;
}

void IConnectorBackEnd::setMode(Mode newMode) {
    _mode = newMode;
}

QSharedPointer<User> IConnectorBackEnd::activeUser() const {
    return _activeUser;
}

void IConnectorBackEnd::setActiveUser(QSharedPointer<User> newActiveUser) {
    _activeUser = newActiveUser;
}

QSharedPointer<Card> IConnectorBackEnd::activeCard() const {
    return _activeCard;
}

void IConnectorBackEnd::setActiveCard(QSharedPointer<Card> newActiveCard,
                                      int purchasesCount) {
    _activeCard = newActiveCard;
    _purchasesCount = purchasesCount;
}

}

