
//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "apiobjectsfactoryv1-5.h"

#include <CheatCard/api/api1-5/userscards.h>

#include <CheatCard/api/api0/contacts.h>

namespace RC {

APIObjectsFactoryV1_5::APIObjectsFactoryV1_5(QH::ISqlDBCache *db):
    APIObjectsFactoryV1(db) {}

#define API_PREFIX APIv1_5

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV1_5::getAllUserData(unsigned int userId) const {
    return getAllUserDataImpl<API_PREFIX::UsersCards>(userId);
}

QSharedPointer<API::UsersCards>
APIObjectsFactoryV1_5::getUserCardData(unsigned int userId, unsigned int cardId) const {
    return getUserCardDataImpl<API_PREFIX::UsersCards>(userId, cardId);

}

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV1_5::getAllUserFromCard(unsigned int cardId) const {
    return getAllUserFromCardImpl<API_PREFIX::UsersCards>(cardId);
}

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV1_5::getAllPassiveUserFromCard(unsigned int cardId,
                                               int unixTimeRange) const {
    return getAllPassiveUserFromCardImpl<API_PREFIX::UsersCards>(cardId, unixTimeRange);
}

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV1_5::getAllActiveUserFromCard(unsigned int cardId,
                                              int unixTimeRange) const {
    return getAllActiveUserFromCardImpl<API_PREFIX::UsersCards>(cardId, unixTimeRange);
}

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV1_5::getAllUserCardsData(const QByteArray &userKey,
                                         const QList<QSharedPointer<API::Contacts>> &childs) {


    QList<QByteArray> keys;

    for (const auto& key: childs) {
        keys.push_back(key->getUserKey());
    }
    keys.push_back(userKey);

    return getAllUserCardsDataImpl<API_PREFIX::UsersCards>(keys);

}

}
