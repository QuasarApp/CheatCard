
//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "rcutils.h"
#include <QHash>
#include <QCryptographicHash>
#include <qaglobalutils.h>

namespace RC {

#pragma pack(push, 1)
struct UserCardId {
    unsigned int cardId;
    unsigned int userId;
};
#pragma pack(pop)

RCUtils::RCUtils()
{

}

unsigned int RCUtils::makeUserId(const QByteArray &userKey) {
    return qHash(userKey);
}

unsigned long long RCUtils::makeUsersCardsId(unsigned int user, unsigned int card) {
    long long id = user;
    id = id << 32;
    id = id | card ;

    return id;
}

unsigned int RCUtils::getUserIdFromUsrsCards(long long userscard) {
    return reinterpret_cast<UserCardId*>(&userscard)->userId;
}

unsigned int RCUtils::getCardIdFromUsrsCards(long long userscard) {
    return reinterpret_cast<UserCardId*>(&userscard)->cardId;
}

QByteArray RCUtils::makeUserKey(const QByteArray &secret) {
    return QCryptographicHash::hash(secret, QCryptographicHash::Sha256);
}

}
