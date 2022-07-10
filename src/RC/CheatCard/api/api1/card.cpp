//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "card.h"
#include <ctime>
#include <CheatCard/api/api0/user.h>

namespace RC {
namespace APIv1 {



Card::Card(): API::Card() {}

QH::PKG::DBObject *Card::createDBObject() const {
    return new Card();
}

QDataStream &Card::fromStream(QDataStream &stream) {
    DBObject::fromStream(stream); // clazy:exclude=skipped-base-method


    stream >> _title;
    stream >> _logo;
    stream >> _seal;
    stream >> _background;
    stream >> _phone;
    stream >> _telegramm;
    stream >> _instagramm;
    stream >> _physicalAddress;
    stream >> _webSite;
    stream >> freeIndex;
    stream >> color;
    stream >> fontColor;
    stream >> _freeItemName;
    stream >> cardVersion;
    stream >> _ownerSignature;

    return stream;
}

QDataStream &Card::toStream(QDataStream &stream) const {
    DBObject::toStream(stream); // clazy:exclude=skipped-base-method


    stream << _title;
    stream << _logo;
    stream << _seal;
    stream << _background;
    stream << _phone;
    stream << _telegramm;
    stream << _instagramm;
    stream << _physicalAddress;
    stream << _webSite;
    stream << freeIndex;
    stream << color;
    stream << fontColor;
    stream << _freeItemName;
    stream << cardVersion;
    stream << _ownerSignature;

    return stream;
}

QString Card::toString() const {
    QString result("id: %0 \n"
                   "title: %1 \n"
                   "phone: %2 \n"
                   "telegram: %3 \n"
                   "instagram: %4 \n"
                   "physicalAddress: %5 \n "
                   "webSite: %6 \n "
                   "freeItemName: %7 \n "
                   "freeIndex: %8 \n "
                   "cardVersion: %9 \n ");

    result = result.arg(cardId()).
            arg(_title,
                _phone,
                _telegramm,
                _instagramm,
                _physicalAddress,
                _webSite,
                _freeItemName).
            arg(freeIndex).
            arg(cardVersion);

    result += ("ownerId: %0 \n");
    result = result.arg(API::User::makeId(_ownerSignature));

    return result;
}


}
}
