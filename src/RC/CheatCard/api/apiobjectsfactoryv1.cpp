
//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "apiobjectsfactoryv1.h"

#include <CheatCard/api/api1/userscards.h>
#include <CheatCard/api/api1/card.h>

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
APIObjectsFactoryV1::getAllUserFromCard(unsigned int cardId) const {
    return getAllUserFromCardImpl<API_PREFIX::UsersCards>(cardId);

}

QSharedPointer<API::Card>
APIObjectsFactoryV1::getCard(unsigned int cardId) {
    return getCardImpl<API_PREFIX::Card>(cardId);

}

QList<QSharedPointer<API::Card> >
APIObjectsFactoryV1::getAllUserCards(const QByteArray &userKey, bool restOf) {
    return getAllUserCardsImpl<API_PREFIX::Card>(userKey, restOf);

}

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV1::getAllUserCardsData(const QByteArray &userKey) {
    return getAllUserCardsDataImpl<API_PREFIX::UsersCards>(userKey);

}
}
