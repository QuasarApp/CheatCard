//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#include "statusafterchanges.h"
#include "CheatCard/api/api1-5/userscards.h"


namespace RC {
namespace APIv1_5 {

StatusAfterChanges::StatusAfterChanges() {

}

RC::APIv1_5::StatusAfterChanges::~StatusAfterChanges() {

}

bool StatusAfterChanges::status() const {
    return _status;
}

void StatusAfterChanges::setStatus(bool newStatus) {
    _status = newStatus;
}

QDataStream &StatusAfterChanges::fromStream(QDataStream &stream) {
    API::Session::fromStream(stream);
    stream >> _status;
    stream >> lastStatus;
    stream >> _neededCard;

    return stream;
}

QDataStream &StatusAfterChanges::toStream(QDataStream &stream) const {
    API::Session::toStream(stream);
    stream << _status;
    stream << lastStatus;
    stream << _neededCard;

    return stream;
}

const QH::PKG::DataPack<APIv1_5::UsersCards> &StatusAfterChanges::getLastStatus() const {
    return lastStatus;
}

void StatusAfterChanges::setLastStatus(const QH::PKG::DataPack<APIv1_5::UsersCards> &newLastStatus) {
    lastStatus = newLastStatus;
}

void StatusAfterChanges::addLastStatus(const QSharedPointer<APIv1_5::UsersCards> &newLastStatus) {
    lastStatus.push(newLastStatus);
}

bool StatusAfterChanges::neededCard() const {
    return _neededCard;
}

void StatusAfterChanges::setNeededCard(bool newNeededCard) {
    _neededCard = newNeededCard;
}

unsigned int StatusAfterChanges::getUser() const {
    return APIv1_5::UsersCards::getUserId(getUsercardId());
}

unsigned int StatusAfterChanges::getCard() const {
    return APIv1_5::UsersCards::getCardId(getUsercardId());
}

}
}
