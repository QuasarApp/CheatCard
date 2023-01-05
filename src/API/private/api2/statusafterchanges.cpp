//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "statusafterchanges.h"

#include "rci/rcutils.h"

namespace RC {
namespace API {
namespace V2 {

StatusAfterChanges::StatusAfterChanges() {

}

StatusAfterChanges::~StatusAfterChanges() {

}

bool StatusAfterChanges::status() const {
    return _status;
}

void StatusAfterChanges::setStatus(bool newStatus) {
    _status = newStatus;
}

QDataStream &StatusAfterChanges::fromStream(QDataStream &stream) {
    API::V0::Session::fromStream(stream);
    stream >> _status;
    stream >> lastStatus;
    stream >> _neededCard;

    return stream;
}

QDataStream &StatusAfterChanges::toStream(QDataStream &stream) const {
    API::V0::Session::toStream(stream);
    stream << _status;
    stream << lastStatus;
    stream << _neededCard;

    return stream;
}

const QH::PKG::DataPack<API::V2::UsersCards> &StatusAfterChanges::getLastStatus() const {
    return lastStatus;
}

void StatusAfterChanges::setLastStatus(const QH::PKG::DataPack<API::V2::UsersCards> &newLastStatus) {
    lastStatus = newLastStatus;
}

void StatusAfterChanges::addLastStatus(const QSharedPointer<API::V2::UsersCards> &newLastStatus) {
    lastStatus.push(newLastStatus);
}

bool StatusAfterChanges::neededCard() const {
    return _neededCard;
}

void StatusAfterChanges::setNeededCard(bool newNeededCard) {
    _neededCard = newNeededCard;
}

unsigned int StatusAfterChanges::getUser() const {
    return RC::RCUtils::getUserIdFromUsrsCards(getUsercardId());
}

unsigned int StatusAfterChanges::getCard() const {
    return RC::RCUtils::getCardIdFromUsrsCards(getUsercardId());
}

}
}
}
