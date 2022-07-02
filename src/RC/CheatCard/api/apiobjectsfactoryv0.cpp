
//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "apiobjectsfactoryv0.h"

#include <CheatCard/api/api0/card.h>
#include <CheatCard/api/api0/user.h>
#include <CheatCard/api/api0/userscards.h>
#include <CheatCard/api/api0/contacts.h>

namespace RC {

APIObjectsFactoryV0::APIObjectsFactoryV0(QH::ISqlDBCache *db): APIObjectsFactory(db) {

}

#define API_PREFIX API
QSharedPointer<API::User> APIObjectsFactoryV0::getUser(unsigned int userId) const {
    return getUserImpl<API_PREFIX::User>(userId);
}

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV0::getAllUserData(unsigned int userId) const {
    return getAllUserDataImpl<API_PREFIX::UsersCards>(userId);

}

QSharedPointer<API::UsersCards>
APIObjectsFactoryV0::getUserCardData(unsigned int userId, unsigned int cardId) const {
    return getUserCardDataImpl<API_PREFIX::UsersCards>(userId, cardId);

}

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV0::getAllUserFromCard(unsigned int cardId) const {
    return getAllUserFromCardImpl<API_PREFIX::UsersCards>(cardId);

}

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV0::getAllPassiveUserFromCard(unsigned int cardId,
                                               int unixTimeRange) const {
    return getAllPassiveUserFromCardImpl<API_PREFIX::UsersCards>(cardId, unixTimeRange);

}

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV0::getAllActiveUserFromCard(unsigned int cardId,
                                              int unixTimeRange) const {
    return getAllActiveUserFromCardImpl<API_PREFIX::UsersCards>(cardId, unixTimeRange);
}

QList<QSharedPointer<API::User> >
APIObjectsFactoryV0::getAllUserDataFromCard(unsigned int cardId) const {
    return getAllUserDataFromCardImpl<API_PREFIX::User>(cardId);

}

QSharedPointer<API::Card>
APIObjectsFactoryV0::getCard(unsigned int cardId) {
    return getCardImpl<API_PREFIX::Card>(cardId);

}

QList<QSharedPointer<API::Card> >
APIObjectsFactoryV0::getAllUserCards(const QByteArray &userKey, bool restOf) {
    return getAllUserCardsImpl<API_PREFIX::Card>(userKey, restOf);

}

QList<QSharedPointer<API::UsersCards> >
APIObjectsFactoryV0::getAllUserCardsData(const QByteArray &userKey) {
    return getAllUserCardsDataImpl<API_PREFIX::UsersCards>(userKey);

}

QSharedPointer<API::Contacts>
APIObjectsFactoryV0::getContactFromChildId(unsigned int userId, unsigned int childUserId) {
    return getContactFromChildIdImpl<API::Contacts>(userId, childUserId);
}

QSharedPointer<API::Contacts>
APIObjectsFactoryV0::getContactFromGenesis(unsigned int userId, int genesis) {
    return getContactFromGenesisIdImpl<API::Contacts>(userId, genesis);

}


}
