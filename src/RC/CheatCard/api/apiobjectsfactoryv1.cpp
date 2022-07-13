
//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "apiobjectsfactoryv1.h"

#include <CheatCard/api/api1/userscards.h>
#include <CheatCard/api/api1/card.h>
#include <CheatCard/api/api0/contacts.h>


namespace RC {

APIObjectsFactoryV1::APIObjectsFactoryV1(QH::ISqlDBCache *db): APIObjectsFactoryV0(db)
{

}

#define API_PREFIX APIv1

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV1::getAllUserData(unsigned int userId) const {
    return getAllUserDataImpl<API_PREFIX::UsersCards>(userId);
}

QSharedPointer<API::UsersCards>
APIObjectsFactoryV1::getUserCardData(unsigned int userId, unsigned int cardId) const {
    return getUserCardDataImpl<API_PREFIX::UsersCards>(userId, cardId);

}

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV1::getAllUserFromCard(unsigned int cardId, unsigned int ignoreuser) const {
    return getAllUserFromCardImpl<API_PREFIX::UsersCards>(cardId, ignoreuser);
}

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV1::getAllPassiveUserFromCard(unsigned int cardId,
                                               int unixTimeRange,
                                               unsigned int ignoreuser) const {
    return getAllPassiveUserFromCardImpl<API_PREFIX::UsersCards>(cardId, unixTimeRange, ignoreuser);
}

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV1::getAllActiveUserFromCard(unsigned int cardId,
                                              int unixTimeRange,
                                              unsigned int ignoreuser) const {
    return getAllActiveUserFromCardImpl<API_PREFIX::UsersCards>(cardId, unixTimeRange, ignoreuser);
}

QSharedPointer<API::Card>
APIObjectsFactoryV1::getCard(unsigned int cardId) {
    return getCardImpl<API_PREFIX::Card>(cardId);

}

QList<QSharedPointer<API::Card> >
APIObjectsFactoryV1::getAllUserCards(const QByteArray &userKey, bool restOf,
                                     const QList<QSharedPointer<API::Contacts> > &childs) {

    QList<QByteArray> keys;

    for (const auto& key: childs) {
        keys.push_back(key->getUserKey());
    }
    keys.push_back(userKey);

    return getAllUserCardsImpl<API_PREFIX::Card>(keys, restOf);

}

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV1::getAllUserCardsData(const QByteArray &userKey,
                                         const QList<QSharedPointer<API::Contacts>> &childs) {


    QList<QByteArray> keys;

    for (const auto& key: childs) {
        keys.push_back(key->getUserKey());
    }
    keys.push_back(userKey);

    return getAllUserCardsDataImpl<API_PREFIX::UsersCards>(keys);

}

}
